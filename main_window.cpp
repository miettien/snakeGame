/*
####################################################################
# TIE-02201 Ohjelmointi 2: Perusteet, K2019                        #
# TIE-02207 Programming 2: Basics, S2019                           #
#                                                                  #
# Project4: Snake: The Revengeance                                 #
# Program description: "While revenge is served cold and vengeance #
#                       is served hot revengeance is often seen    #
#                       served with Cajun with a side of onion     #
#                       rings and often dumplings in the Orient."  #
#                       - urbandictionary                          #
#                                                                  #
# File: main_window.cpp                                            #
# Description: Defines a class implementing a UI for the game.     #
#                                                                  #
# Author: Miro Miettinen          				   #
####################################################################
*/

#include "main_window.hh"
#include <QGraphicsRectItem>
#include <QKeyEvent>


MainWindow::MainWindow(QWidget* parent):
    QMainWindow(parent) {

    ui_.setupUi(this);
    ui_.graphicsView->setScene(&scene_);

    ui_.radioButtonEasy->setChecked(true);
    ui_.radioButtonSmall->setChecked(true);

    ui_.deadTimeLabel->setHidden(true);
    ui_.deadLabel->setHidden(true);
    ui_.winLabel->setHidden(true);

    connect(&timer_, &QTimer::timeout, this, &MainWindow::moveSnake);
    connect(&elapsed_timer_, &QTimer::timeout, this, &MainWindow::updateLcdTime);
}

void MainWindow::keyPressEvent(QKeyEvent* event) {

    //Read the event to see which key got pressed and store the result.
    if(event->key() == Qt::Key_W){
        //dont do anything if user is trying to move backwards
        if(direction_ == "down" and body_parts_.size() > 0){
            return;
        }
        direction_ = "up";
        return;

    } else if(event->key() == Qt::Key_S){
        //dont do anything if user is trying to move backwards
        if(direction_ == "up" and body_parts_.size() > 0){
            return;
        }
        direction_ = "down";
        return;
    } else if(event->key() == Qt::Key_D){
        //dont do anything if user is trying to move backwards
        if(direction_ == "right" and body_parts_.size() > 0){
            return;
        }
        direction_ = "left";
        return;
    } else if(event->key() == Qt::Key_A){
        //dont do anything if user is trying to move backwards
        if(direction_ == "left" and body_parts_.size() > 0){
            return;
        }
        direction_ = "right";
        return;
    }
}

void MainWindow::addRects()
{
    //food rectangle
    const QRect food_rect(0, 0, 1, 1);
    const QPen pen(Qt::black, 0);
    const QBrush brush(Qt::yellow);
    food_ = scene_.addRect(food_rect, pen, brush);

    //head rectanlge
    const QRect head_rect(0,0,1,1);
    const QPen head_pen(Qt::red, 0);
    const QBrush head_brush(Qt::red);
    head_ = scene_.addRect(head_rect, head_pen, head_brush);
}

void MainWindow::on_playButton_clicked() {
    ui_.playButton->setDisabled(true);
    ui_.pauseButton->setDisabled(false);
    ui_.difficultyWidget->setDisabled(true);
    ui_.sizeWidget->setDisabled(true);
    ui_.pauseButton->setText("Pause");

    addRects();
    head_->setPos(area_size_/2, area_size_/2);

    adjustSceneArea();

    timer_.start(speed_);
    elapsed_timer_.start(1000);

    moveFood();
}

void MainWindow::moveSnake() {
    const QPointF old_head_pos = head_->scenePos();
    QPointF new_head_pos;

    //Move head
    if(direction_ == "up"){
        new_head_pos = old_head_pos + QPoint(0, -1);
    } else if(direction_ == "down"){
        new_head_pos = old_head_pos + QPoint(0, 1);
    } else if(direction_ == "left"){
        new_head_pos = old_head_pos + QPoint(1, 0);
    } else if(direction_ == "right"){
        new_head_pos = old_head_pos + QPoint(-1, 0);

    }

    isGameLost(new_head_pos);
    head_->setPos(new_head_pos);

    if(foundFood(old_head_pos)){
        if(isGameWon()){
            return;
        }
        moveFood();
    }
    //Moves the rest of the body with head
    for (int i = 0; i < body_parts_.size() - 1; ++i){
        body_parts_[i]->setPos(body_parts_[i+1]->pos());
    }
    if(body_parts_.size() > 0){
        body_parts_.last()->setPos(old_head_pos);
    }
}

void MainWindow::adjustSceneArea() {

    const QRectF area(0, 0, area_size_, area_size_);
    scene_.setSceneRect(area);
    ui_.graphicsView->fitInView(area);

}

bool MainWindow::foundFood(const QPointF old_head_pos)
{
    if(head_->scenePos() == food_->scenePos()){
        isGameWon();

        //makes a new piece of body and adds it to QList
        const QRect body_rect(0,0,1,1);
        const QPen body_pen(Qt::red, 0);
        const QBrush body_brush(Qt::green);
        body_part_ = scene_.addRect(body_rect, body_pen, body_brush);
        body_part_->setPos(old_head_pos);

        body_parts_.prepend(body_part_);

        //make snake move faster after eating
        speed_ = speed_ * difficulty_;
        timer_.start(speed_);
        return true;
    }
    return false;
}

void MainWindow::moveFood()
{
    std::uniform_int_distribution<int> width_dist(0, area_size_ -1);
    std::uniform_int_distribution<int> height_dist(0, area_size_ -1);

    while (true) {
        QPointF new_food_pos = QPoint(width_dist(rng_),height_dist(rng_));

        //check if head is in the position and tries again if it is
        if(new_food_pos == head_->pos()){
            continue;
        }
        //check if body part in the position and tries again if it is
        for (auto part : body_parts_) {
            if (new_food_pos == part->pos()){
                continue;
            }
        }
        //empty place found
            food_->setPos(new_food_pos);
            break;
    }
}

void MainWindow::isGameLost(QPointF new_head_pos)
{
    //check if snake tries to eat itself
    for(auto part : body_parts_){
        if(new_head_pos == part->pos()){
            endGame(false);
        }
    }
    //if collision to a wall
    if(new_head_pos.x() < 0 or new_head_pos.y() < 0 or
            new_head_pos.x() >= area_size_ or new_head_pos.y() >= area_size_){
        endGame(false);
    }
}

bool MainWindow::isGameWon(){

    // check if the whole area is filled with snake
    if(body_parts_.size() + 1 == pow(area_size_, 2)){
        endGame(true);
        return true;
    }
    return false;
}

void MainWindow::endGame(bool is_won)
{
    if(!is_won){
        timer_.stop();
        elapsed_timer_.stop();

        // Dead graphics
        const QRect dead_rect(0,0, area_size_, area_size_);
        const QPen dead_pen(Qt::red, 1);
        const QBrush dead_brush(Qt::black);
        scene_.addRect(dead_rect, dead_pen, dead_brush);

        ui_.deadTimeLabel->setHidden(false);
        ui_.deadLabel->setHidden(false);
    } else{
        timer_.stop();
        elapsed_timer_.stop();

        // Win graphics
        const QRect win_rect(0,0, area_size_, area_size_);
        const QPen win_pen(Qt::green, 0);
        const QBrush win_brush(Qt::green);
        scene_.addRect(win_rect, win_pen, win_brush);

        ui_.winLabel->setHidden(false);
        ui_.deadTimeLabel->setHidden(false);
    }
}

void MainWindow::updateLcdTime()
{
    ui_.lcdTime->display(elapsed_time_);
    ++elapsed_time_;
}

void MainWindow::on_quitButton_clicked()
{
    close();
}

void MainWindow::on_restartButton_clicked()
{
    timer_.stop();

    elapsed_timer_.stop();
    elapsed_time_ = 0;
    updateLcdTime();

    ui_.graphicsView->items().clear();
    body_parts_.clear();
    scene_.clear();

    ui_.playButton->setDisabled(false);
    ui_.pauseButton->setDisabled(true);

    ui_.difficultyWidget->setDisabled(false);
    ui_.sizeWidget->setDisabled(false);

    ui_.deadTimeLabel->setHidden(true);
    ui_.deadLabel->setHidden(true);
    ui_.winLabel->setHidden(true);

    direction_ = "down";
    speed_ = 1000;
}

void MainWindow::on_pauseButton_clicked()
{
    if(timer_on_){
        timer_.stop();
        elapsed_timer_.stop();
        timer_on_ = false;

        ui_.pauseButton->setText("Resume");

    } else{
        timer_.start(speed_);
        elapsed_timer_.start(1000);
        timer_on_ = true;

        ui_.pauseButton->setText("Pause");
    }
}

void MainWindow::on_radioButtonEasy_toggled(bool checked)
{

    if(checked){
        difficulty_ = 1;
    }

}

void MainWindow::on_radioButtonMedium_toggled(bool checked)
{
    if(checked){
        difficulty_ = 0.97;
    }
}

void MainWindow::on_radioButtonHard_toggled(bool checked)
{
    if(checked){
        difficulty_ = 0.95;
    }
}

void MainWindow::on_radioButtonSmall_toggled(bool checked)
{
    if(checked){
        area_size_ = 7;
    }
}

void MainWindow::on_radioButtonMediumS_toggled(bool checked)
{
    if(checked){
        area_size_ = 10;
    }
}

void MainWindow::on_radioButtonBig_toggled(bool checked)
{
    if(checked){
        area_size_ = 13;
    }
}
