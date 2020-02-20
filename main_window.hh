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
# File: main_window.hh                                             #
# Description: Declares a class implementing a UI for the game.    #
#                                                                  #
# Author: Miro Miettinen			                   #
####################################################################
*/

#ifndef PRG2_SNAKE2_MAINWINDOW_HH
#define PRG2_SNAKE2_MAINWINDOW_HH

#include "ui_main_window.h"
#include <QGraphicsScene>
#include <QMainWindow>
#include <QTimer>
#include <random>
#include <QList>
#include <QPointF>
#include <cmath>


/* \class MainWindow
 * \brief Implements the main window through which the game is played.
 */
class MainWindow: public QMainWindow {
    Q_OBJECT

public:

    /* \brief Construct a MainWindow.
     *
     * \param[in] parent The parent widget of this MainWindow.
     */
    explicit MainWindow(QWidget* parent = nullptr);

    /* \brief Destruct a MainWindow.
     */
    ~MainWindow() override = default;

    /* \brief Change the Snake's bearing when certain keys get pressed.
     *
     * \param[in] event Contains data on pressed and released keys.
     */
    void keyPressEvent(QKeyEvent* event) override;


private slots:

    /* \brief sets all rects needed at the start of the game
     */
    void addRects();
    /* \brief Start the game.
     */
    void on_playButton_clicked();

    /* \brief Move the Snake by a square and check for collisions.
     *
     * The game ends if a wall or the Snake itself gets in the way.
     * When a food gets eaten a point is gained and the Snake grows.
     */
    void moveSnake();

    /* \brief Quit the game.
     */
    void on_quitButton_clicked();

    /* \brief Restart the game. And resets the settings
     */
    void on_restartButton_clicked();

    /* \brief Pause/resume the game.
      */
    void on_pauseButton_clicked();

    /* \brief Set difficultiness easy
      */
    void on_radioButtonEasy_toggled(bool checked);
    /* \brief Set difficultiness medium
      */
    void on_radioButtonMedium_toggled(bool checked);
    /* \brief Set difficultiness hard
      */
    void on_radioButtonHard_toggled(bool checked);
    /* \brief Set game area's size small
      */
    void on_radioButtonSmall_toggled(bool checked);
    /* \brief Set game area's size medium
      */
    void on_radioButtonMediumS_toggled(bool checked);
    /* \brief Set game area's size big
      */
    void on_radioButtonBig_toggled(bool checked);

private:

    /* \brief Make the play field visible and fit it into the view.
     *
     * Should be called every time the field's size changes.
     */
    void adjustSceneArea();

    /* \brief cheks if snake has found food and returns true if yes
     *
     * param[in] old position of snake's head
     */
    bool foundFood(const QPointF old_head_pos);

    /* \brief Finds new place to food when eaten
     */
    void moveFood();
    /* \brief Checks if the gamer has lost for some reason
     *
     * param[in] new position of head = where the snake is trying to move next
      */
    void isGameLost(QPointF new_head_pos);
    /* \brief Checks if the game is won and triggers endGame if true
     */
    bool isGameWon();
    /* \brief Ends game and shows correct grapchis depending on win or lost
     *
     * param[in] bool game is won
     */
    void endGame(bool is_won);
    //updates seconds to the lcd display on ui
    void updateLcdTime();

    Ui::MainWindow ui_;                 /**< Accesses the UI widgets. */
    QGraphicsScene scene_;              /**< Manages drawable objects. */

    QGraphicsRectItem* food_ = nullptr; /**< The food item in the scene. */
    QGraphicsRectItem* head_ = nullptr; /**< The head item in the scene */
    QGraphicsRectItem* body_part_ = nullptr; /** The body item in the scene */

    QTimer timer_;                      /**< Triggers the Snake to move. */
    QTimer elapsed_timer_;              /**< Triggers seconds to add up */
    int elapsed_time_ = 0;              /**< Calculates seconds spent*/
    bool timer_on_ = true;              /**< Tells if the timer is on*/

    std::default_random_engine rng_;    /**< Randomizes food locations. */

    std::string direction_ = "up";      /**< Tells the direction for the snake*/
    int area_size_ = 7;                 /**< The size of game area */
    double speed_ = 1000;               /** Manages the speed of the snake*/
    double difficulty_ = 1;             /** Multiplier for the speed (<=1)*/


    QList<QGraphicsRectItem*> body_parts_; /** Contains snake's body */

};  // class MainWindow


#endif  // PRG2_SNAKE2_MAINWINDOW_HH
