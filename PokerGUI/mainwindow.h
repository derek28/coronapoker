#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QTcpSocket>
#include "game_state.h"
#include <QLabel>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:

    void myfunction();

    void readFromSocket();

    void on_spinBox_valueChanged(int arg1);

    void on_horizontalSlider_valueChanged(int value);

    void on_actionabout_QT_triggered();

    void on_actionabout_this_triggered();

    void on_actionconnect_triggered();

    void on_VillainCardOn_clicked(bool checked);

    void on_RAISEBTN_clicked();

    void on_CALLBTN_clicked();

    void on_FOLDBTN_clicked();

private:
    void updateRange(int i, int j);
    void fillCard(QLabel* label, Card card);
    void sendAction(Action);
    void updateGameState();
    void updateLegalActions();
    void updateCards();
    void removeCard(QLabel* label);

    Ui::MainWindow *ui;
    QTimer *timer;
    QTcpSocket socket;
    QByteArray buffer;
    bool villain_card_on = false;
    GameStateNoVector* gs;
    LegalActionsSimplify* la;

    QByteArray game_state_buf;
    QByteArray legal_action_buf;

};

#endif // MAINWINDOW_H
