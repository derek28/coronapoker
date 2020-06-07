#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QPixmap>
#include <QDir>
#include <QDebug>
#include <QHostAddress>
#include <QTime>
#include "game_state.h"
#include "mainwindow.h"
//#include <QSound>
#include "game.pb.h"
#include <QScrollBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    //display to-be-connected info
    ui->statusBar->showMessage("please connect to host");
    //Setup timer function
    timer = new QTimer(this);
    connect(timer,SIGNAL(timeout()),this,SLOT(myfunction()));
    timer->start(2000);

    ui->RAISEBTN->setEnabled(false);
    ui->CALLBTN->setEnabled(false);
    ui->FOLDBTN->setEnabled(false);

    //test set range
    updateRange(0,10000);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::readFromSocket()
{
    qDebug() << "buffer size before reading socket: " << buffer.size() ;
    buffer.append(socket.readAll());
    qDebug() << "buffer size after reading socket: " << buffer.size();
    static int size = 0 ;
    while ((size == 0 && buffer.size() >= 4) || (size > 0 && buffer.size() >= size)) //While can process data, process it
    {
        if (size == 0 && buffer.size() >= 4) //if size of data has received completely, then store it on our global variable
        {
            memcpy (&size, buffer.data(), 4);
            qDebug() << "payload size:" << size;
            buffer.remove(0, 4);
        }
        if (size > 0 && buffer.size() >= size-4) // If data has received completely, process the data
        {

            qDebug() << "payload first index:" << (int) buffer[0];
            if ( static_cast<unsigned char>(buffer[0]) == static_cast<unsigned char>(1) ) { // if data is gamestate
                buffer.remove(0,1);
                game_state_buf = buffer.mid(0,size-5);
                buffer.remove(0, size-5);
                updateGameState();
            }
            else if ( static_cast<unsigned char>(buffer[0]) == static_cast<unsigned char>(2) ){
                buffer.remove(0,1);
                legal_action_buf = buffer.mid(0,size-5);
                buffer.remove(0, size-5);
                updateLegalActions();
            }
            else if ( static_cast<unsigned char>(buffer[0]) == static_cast<unsigned char>(3) ){
                buffer.remove(0,1);
                actionwithid_buf = buffer.mid(0,size-5);
                buffer.remove(0, size-5);
                updateActionWithID();
            }
            else if ( static_cast<unsigned char>(buffer[0]) == static_cast<unsigned char>(4) ){
                buffer.remove(0,1);
                actionhis_buf = buffer.mid(0,size-5);
                buffer.remove(0, size-5);
                updateActionHistory();
            }
            else if ( static_cast<unsigned char>(buffer[0]) == static_cast<unsigned char>(5) ){
                buffer.remove(0,1);
                gameresult_buf = buffer.mid(0,size-5);
                buffer.remove(0, size-5);
                updateGameResult();
            }
            else {
                qDebug() << "something wrong, payload first index is" << buffer[0];
            }

            size = 0;
        }
    }
}

void MainWindow::updateGameState()
{
    qDebug() << "Update GameState" ;
    gs = (GameStateNoVector*) game_state_buf.data();
    //gs->printdebug();
    //update button position
    if( gs->btn_pos == 0 ) {
        ui->ButtonHero->setVisible(true);
        ui->ButtonVillain->setVisible(false);
    }
    else {
        ui->ButtonHero->setVisible(false);
        ui->ButtonVillain->setVisible(true);
    }
    //update hand number
    ui->Handnumber2->setText(QString::number(gs->hand_number));

    //update xxx thinking
    if (gs->next_player_to_act == 0) {
        ui->YourTurnToAct->setVisible(true);
        ui->VillainTurn->setVisible(false);
    }

    if (gs->next_player_to_act == 1) {
       ui->YourTurnToAct->setVisible(false);
       ui->VillainTurn->setVisible(true);
    }

    //Update the money values
    ui->BetAmountHero->setText(QString::number(gs->bet_ring[0]));
    ui->BetAmountVillain->setText(QString::number(gs->bet_ring[1]));

    ui->StackSizeHero->setText(QString::number(gs->stack_size[0]));
    ui->StackSizeVillain->setText(QString::number(gs->stack_size[1]));


    ui->BankRollHero->setText(QString::number(gs->nb_of_buyins[0]));
    ui->BankRollVillain->setText(QString::number(gs->nb_of_buyins[1]));


    ui->PotSize->setText(QString::number(gs->pot_size));
    ui->TotalPotSize->setText(QString::number(gs->total_pot_size));
    updateCards();
}

void MainWindow::updateGameResult() {
    qDebug() << "Update GameResult" ;

    GameResult* gr = (GameResult*) gameresult_buf.data();

    //remove "bot/hero thinking" due to bug
    ui->VillainTurn->setVisible(false);
    ui->YourTurnToAct->setVisible(false);

    //show winner notification
    if (gr->winner[0]==0 || gr->winner[1]==0) {
        ui->HeroWin->setVisible(true);
    }
    if (gr->winner[0]==1 || gr->winner[1]==1) {
        ui->VillainWin->setVisible(true);
    }

    //decide if villain shows his hand
    if ( (gr->agressor==1 || gr->winner[0] == 1 || gr->winner[1] == 1) && gs->num_player_in_hand>=2) {
        fillCard(ui->HoleCard0Villain,gs->player_hole_cards[1][0]);
        fillCard(ui->HoleCard1Villain,gs->player_hole_cards[1][1]);
    }
    //let the next game button be shown
    ui->pushButtonNextGame->setVisible(true);

}

void MainWindow::updateActionWithID()
{
    qDebug() << "Update ActionWithID" ;
    qDebug() << "To be completed" ;
    return;
}

void MainWindow::updateActionHistory()
{
    ui->textBrowser->setText(actionhis_buf);
    ui->textBrowser->verticalScrollBar()->setValue(ui->textBrowser->verticalScrollBar()->maximum());
    return;
}

void MainWindow::updateLegalActions()
{
    qDebug() << "Update LegalActions" ;
    la = (LegalActionsSimplify*) legal_action_buf.data();
    if(la->legal_action[2].amount == -1 || la->legal_action[3].amount == -1)
        ui->RAISEBTN->setEnabled(false);
    else{
        ui->RAISEBTN->setEnabled(true);
        updateRange(la->legal_action[2].amount, la->legal_action[3].amount);
        ui->spinBox->setValue(la->legal_action[2].amount);
    }

    if(la->legal_action[1].amount == 0){
        QString calltext("CHECK");
        ui->CALLBTN->setText(calltext);
    }
    else{
    QString calltext("CALL\n");
    calltext.append(QString::number(la->legal_action[1].amount));
    ui->CALLBTN->setText(calltext);
    }

    ui->YourTurnToAct->setVisible(true);
    ui->VillainTurn->setVisible(false);

    ui->RAISEBTN->setEnabled(true);
    ui->CALLBTN->setEnabled(true);
    ui->FOLDBTN->setEnabled(true);
    ui->pushButtonNextGame->setVisible(false);
    ui->HeroWin->setVisible(false);
    ui->VillainWin->setVisible(false);
}

void MainWindow::updateCards(){
    gs = (GameStateNoVector*) game_state_buf.data();

    removeCard(ui->HoleCard0Villain);
    removeCard(ui->HoleCard1Villain);
    removeCard(ui->HoleCard0Hero);
    removeCard(ui->HoleCard1Hero);
    removeCard(ui->CommCard0);
    removeCard(ui->CommCard1);
    removeCard(ui->CommCard2);
    removeCard(ui->CommCard3);
    removeCard(ui->CommCard4);

    if(villain_card_on == true){
        fillCard(ui->HoleCard0Villain,gs->player_hole_cards[1][0]);
        fillCard(ui->HoleCard1Villain,gs->player_hole_cards[1][1]);
    }
    else {
    }

    fillCard(ui->HoleCard0Hero,gs->player_hole_cards[0][0]);
    fillCard(ui->HoleCard1Hero,gs->player_hole_cards[0][1]);

    if(gs->community_size >= 3) {
        fillCard(ui->CommCard0, gs->community_cards[0]);
        fillCard(ui->CommCard1, gs->community_cards[1]);
        fillCard(ui->CommCard2, gs->community_cards[2]);
    }
    if(gs->community_size >= 4) {
        fillCard(ui->CommCard3, gs->community_cards[3]);
    }
    if(gs->community_size == 5) {
        fillCard(ui->CommCard4, gs->community_cards[4]);
    }
}



void MainWindow::updateRange(int i, int j){
    //update bet range
    ui->spinBox->setRange(i,j);
    ui->horizontalSlider->setRange(i,j);
}

void MainWindow::fillCard(QLabel* label, Card card){

    const int w = label->width();
    const int h = label->height();

    // Fill holecards
    QString path = QString (":/resources/bmp/%1-%2.svg").arg(card.GetRank()).arg(card.GetSuit());
    QPixmap pix(path);
    label->setPixmap(pix.scaled(w,h,Qt::KeepAspectRatio));
    return;
}

void MainWindow::removeCard(QLabel* label){
    label->setPixmap(QPixmap());
    return;
}


void MainWindow::sendAction(Action playerAction){
    char outbuf[1024] = {0};
    memcpy(outbuf, &playerAction, sizeof(playerAction));
    int status = socket.write(outbuf,sizeof(playerAction));
    qDebug() << "action sent. Sent status (Byte count):" << status;

    ui->YourTurnToAct->setVisible(false);
    ui->VillainTurn->setVisible(true);
    ui->RAISEBTN->setEnabled(false);
    ui->CALLBTN->setEnabled(false);
    ui->FOLDBTN->setEnabled(false);
}

void MainWindow::myfunction()
{
    qDebug() << "Time: " << QTime::currentTime().toString("hh:mm:ss");
    qDebug() << "socket state is" << (QAbstractSocket::SocketState)socket.state();
    qDebug() << "incoming buffer size: " << buffer.size();

}

void MainWindow::on_spinBox_valueChanged(int arg1)
{
    ui->horizontalSlider->setValue(arg1);

    QString raisetext("RAISE/BET\n");
    raisetext.append(QString::number(arg1));
    ui->RAISEBTN->setText(raisetext);
}

void MainWindow::on_horizontalSlider_valueChanged(int value)
{
    ui->spinBox->setValue(value);
}

void MainWindow::on_actionabout_QT_triggered()
{
    QMessageBox::aboutQt(this,"About QT");
}

void MainWindow::on_actionabout_this_triggered()
{
    return;
}

void MainWindow::on_actionconnect_triggered()
{
    //connect to server
    socket.waitForReadyRead(5000);
    socket.connectToHost(QHostAddress("127.0.0.1"),8088);
    qDebug() << "conneting to server" << QTime::currentTime().toString("hh:mm:ss");
    connect(&socket, SIGNAL(readyRead()),this, SLOT(readFromSocket()));
    //show text on status bar
    ui->statusBar->showMessage("connected to: 127.0.0.1:8088");
}

void MainWindow::on_VillainCardOn_clicked(bool checked)
{
    if (checked){
        qDebug() << "show vil hand";
        villain_card_on = 1;
    }

    else {
        qDebug() << "do not show vil hand";
        villain_card_on = 0;
    }
    updateCards();

}

void MainWindow::on_RAISEBTN_clicked()
{
    Action playerAction;
    playerAction.action = 2;
    playerAction.amount = ui->spinBox->value();
    sendAction(playerAction);
}

void MainWindow::on_CALLBTN_clicked()
{
    Action playerAction;
    playerAction.action = 1;
    playerAction.amount = la->legal_action[1].amount;
    sendAction(playerAction);
}

void MainWindow::on_FOLDBTN_clicked()
{
    Action playerAction;
    playerAction.action = 0;
    playerAction.amount = 0;
    sendAction(playerAction);
}

void MainWindow::on_pushButtonNextGame_clicked()
{
    ui->pushButtonNextGame->setVisible(false);
    Action playerAction;
    sendAction(playerAction); //send a meaningless signal for host to continue
}
