#ifndef STUDENTEXAMWINDOW_H
#define STUDENTEXAMWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include <QPushButton>
#include <QRadioButton>
#include <QTextEdit>
#include <QTimer>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QButtonGroup>
#include <QMessageBox>
#include <QListWidget>
#include <QGroupBox>

#include <vector>
#include <string>

#include "TestAttempt.h"
#include "StudentAnswer.h"
#include "Result.h"

struct ExamQuestion
{
    int questionId;
    std::string questionText;
    std::vector<std::string> options;
    int correctOption;
    double score;
    bool isWritten;
    std::string correctText;
};

class StudentExamWindow : public QMainWindow
{
    Q_OBJECT

private:
    // ===== Main pages =====
    QWidget* centralWidget;
    QVBoxLayout* mainLayout;

    // ===== View Test =====
    QLabel* titleLabel;
    QLabel* descriptionLabel;
    QLabel* subjectLabel;
    QLabel* durationLabel;
    QLabel* statusLabel;
    QPushButton* startButton;

    // ===== Take Test =====
    QLabel* questionNumberLabel;
    QLabel* questionLabel;
    QLabel* timerLabel;

    QRadioButton* optionA;
    QRadioButton* optionB;
    QRadioButton* optionC;
    QRadioButton* optionD;

    QTextEdit* writtenAnswer;

    QButtonGroup* optionGroup;

    QPushButton* previousButton;
    QPushButton* nextButton;
    QPushButton* submitButton;

    // ===== Result =====
    QLabel* scoreLabel;
    QLabel* percentageLabel;
    QLabel* gradeLabel;
    QLabel* correctLabel;
    QLabel* wrongLabel;
    QLabel* unansweredLabel;
    QLabel* timeTakenLabel;
    QLabel* submittedAtLabel;

    QPushButton* backButton;

    // ===== Timer =====
    QTimer* examTimer;
    int remainingSeconds;
    int elapsedSeconds;

    // ===== Exam data =====
    std::vector<ExamQuestion> questions;
    std::vector<StudentAnswer> answers;

    int currentQuestion;
    int studentId;
    int testId;

    TestAttempt* currentAttempt;
    Result* currentResult;

    // ===== Functions =====
    void setupViewTestPage();
    void setupTakeTestPage();
    void setupResultPage();

    void showViewTestPage();
    void showTakeTestPage();
    void showResultPage();

    void loadQuestion();
    void saveCurrentAnswer();

    void startExam();
    void submitExam();
    void autoSubmit();

    void calculateResult();

    void updateTimer();

public:
    explicit StudentExamWindow(QWidget* parent = nullptr);
    ~StudentExamWindow();
};

#endif