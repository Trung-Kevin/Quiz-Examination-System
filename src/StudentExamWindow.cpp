#include "StudentExamWindow.h"

#include <QWidget>
#include <QDateTime>
#include <QString>
#include <QFont>

// ============================================================
// CONSTRUCTOR
// ============================================================

StudentExamWindow::StudentExamWindow(QWidget* parent)
    : QMainWindow(parent),
      centralWidget(new QWidget(this)),
      mainLayout(new QVBoxLayout()),

      titleLabel(nullptr),
      descriptionLabel(nullptr),
      subjectLabel(nullptr),
      durationLabel(nullptr),
      statusLabel(nullptr),
      startButton(nullptr),

      questionNumberLabel(nullptr),
      questionLabel(nullptr),
      timerLabel(nullptr),

      optionA(nullptr),
      optionB(nullptr),
      optionC(nullptr),
      optionD(nullptr),

      writtenAnswer(nullptr),
      optionGroup(nullptr),

      previousButton(nullptr),
      nextButton(nullptr),
      submitButton(nullptr),

      scoreLabel(nullptr),
      percentageLabel(nullptr),
      gradeLabel(nullptr),
      correctLabel(nullptr),
      wrongLabel(nullptr),
      unansweredLabel(nullptr),
      timeTakenLabel(nullptr),
      submittedAtLabel(nullptr),

      backButton(nullptr),

      examTimer(new QTimer(this)),
      remainingSeconds(300),
      elapsedSeconds(0),

      currentQuestion(0),
      studentId(1),
      testId(1),

      currentAttempt(nullptr),
      currentResult(nullptr)
{
    setWindowTitle("Quiz Examination System - Student");
    resize(800, 600);

    // ========================================================
    // SAMPLE QUESTIONS
    // ========================================================

    ExamQuestion q1;
    q1.questionId = 1;
    q1.questionText =
        "Which protocol is used to assign IP addresses automatically?";
    q1.options = {
        "HTTP",
        "DHCP",
        "FTP",
        "SSH"
    };
    q1.correctOption = 2;
    q1.score = 2.0;
    q1.isWritten = false;
    q1.correctText = "";

    ExamQuestion q2;
    q2.questionId = 2;
    q2.questionText =
        "Which device is commonly used to connect different networks?";
    q2.options = {
        "Switch",
        "Hub",
        "Router",
        "Repeater"
    };
    q2.correctOption = 3;
    q2.score = 2.0;
    q2.isWritten = false;
    q2.correctText = "";

    ExamQuestion q3;
    q3.questionId = 3;
    q3.questionText =
        "Which protocol is commonly used for secure remote login?";
    q3.options = {
        "FTP",
        "SSH",
        "HTTP",
        "DNS"
    };
    q3.correctOption = 2;
    q3.score = 2.0;
    q3.isWritten = false;
    q3.correctText = "";

    ExamQuestion q4;
    q4.questionId = 4;
    q4.questionText =
        "What does LAN stand for?";
    q4.options = {
        "Local Area Network",
        "Large Access Network",
        "Local Access Node",
        "Long Area Network"
    };
    q4.correctOption = 1;
    q4.score = 2.0;
    q4.isWritten = false;
    q4.correctText = "";

    ExamQuestion q5;
    q5.questionId = 5;
    q5.questionText =
        "Which protocol is used to translate domain names into IP addresses?";
    q5.options = {
        "DNS",
        "DHCP",
        "SMTP",
        "SSH"
    };
    q5.correctOption = 1;
    q5.score = 2.0;
    q5.isWritten = false;
    q5.correctText = "";

    questions.push_back(q1);
    questions.push_back(q2);
    questions.push_back(q3);
    questions.push_back(q4);
    questions.push_back(q5);

    // Create StudentAnswer for each question
    for (size_t i = 0; i < questions.size(); ++i)
    {
        answers.emplace_back(
            static_cast<int>(i + 1),
            0,
            questions[i].questionId
        );
    }

    // ========================================================
    // MAIN WINDOW
    // ========================================================

    centralWidget->setLayout(mainLayout);
    setCentralWidget(centralWidget);

    setupViewTestPage();

    connect(
        startButton,
        &QPushButton::clicked,
        this,
        &StudentExamWindow::startExam
    );

    connect(
        examTimer,
        &QTimer::timeout,
        this,
        &StudentExamWindow::updateTimer
    );
}

// ============================================================
// DESTRUCTOR
// ============================================================

StudentExamWindow::~StudentExamWindow()
{
    delete currentAttempt;
    delete currentResult;
}

// ============================================================
// VIEW TEST PAGE
// ============================================================

void StudentExamWindow::setupViewTestPage()
{
    titleLabel =
        new QLabel("Test: Computer Networks");

    descriptionLabel =
        new QLabel("Basic Computer Networks Examination");

    subjectLabel =
        new QLabel("Subject: Computer Networks");

    durationLabel =
        new QLabel("Duration: 5 minutes");

    statusLabel =
        new QLabel("Status: Published - Available");

    startButton =
        new QPushButton("Start Test");

    titleLabel->setFont(
        QFont("Arial", 18, QFont::Bold)
    );

    mainLayout->addWidget(titleLabel);
    mainLayout->addWidget(descriptionLabel);
    mainLayout->addWidget(subjectLabel);
    mainLayout->addWidget(durationLabel);
    mainLayout->addWidget(statusLabel);

    mainLayout->addSpacing(20);

    mainLayout->addWidget(startButton);
}

// ============================================================
// SHOW VIEW TEST PAGE
// ============================================================

void StudentExamWindow::showViewTestPage()
{
    while (QLayoutItem* item = mainLayout->takeAt(0))
    {
        if (item->widget())
        {
            delete item->widget();
        }

        delete item;
    }

    setupViewTestPage();

    connect(
        startButton,
        &QPushButton::clicked,
        this,
        &StudentExamWindow::startExam
    );
}

// ============================================================
// TAKE TEST PAGE
// ============================================================

void StudentExamWindow::setupTakeTestPage()
{
    while (QLayoutItem* item = mainLayout->takeAt(0))
    {
        if (item->widget())
        {
            delete item->widget();
        }

        delete item;
    }

    questionNumberLabel =
        new QLabel();

    questionLabel =
        new QLabel();

    timerLabel =
        new QLabel();

    optionA =
        new QRadioButton();

    optionB =
        new QRadioButton();

    optionC =
        new QRadioButton();

    optionD =
        new QRadioButton();

    writtenAnswer =
        new QTextEdit();

    optionGroup =
        new QButtonGroup(this);

    optionGroup->addButton(optionA, 1);
    optionGroup->addButton(optionB, 2);
    optionGroup->addButton(optionC, 3);
    optionGroup->addButton(optionD, 4);

    previousButton =
        new QPushButton("Previous");

    nextButton =
        new QPushButton("Next");

    submitButton =
        new QPushButton("Submit Test");

    questionLabel->setWordWrap(true);

    questionLabel->setFont(
        QFont("Arial", 14, QFont::Bold)
    );

    timerLabel->setFont(
        QFont("Arial", 12, QFont::Bold)
    );

    mainLayout->addWidget(questionNumberLabel);
    mainLayout->addWidget(timerLabel);

    mainLayout->addSpacing(10);

    mainLayout->addWidget(questionLabel);

    mainLayout->addWidget(optionA);
    mainLayout->addWidget(optionB);
    mainLayout->addWidget(optionC);
    mainLayout->addWidget(optionD);

    writtenAnswer->setPlaceholderText(
        "Enter your answer..."
    );

    writtenAnswer->hide();

    mainLayout->addWidget(writtenAnswer);

    QHBoxLayout* buttonLayout =
        new QHBoxLayout();

    buttonLayout->addWidget(previousButton);
    buttonLayout->addWidget(nextButton);
    buttonLayout->addWidget(submitButton);

    mainLayout->addLayout(buttonLayout);

    connect(
        previousButton,
        &QPushButton::clicked,
        [this]()
        {
            saveCurrentAnswer();

            if (currentQuestion > 0)
            {
                currentQuestion--;
                loadQuestion();
            }
        }
    );

    connect(
        nextButton,
        &QPushButton::clicked,
        [this]()
        {
            saveCurrentAnswer();

            if (currentQuestion <
                static_cast<int>(questions.size()) - 1)
            {
                currentQuestion++;
                loadQuestion();
            }
        }
    );

    connect(
        submitButton,
        &QPushButton::clicked,
        this,
        &StudentExamWindow::submitExam
    );
}

// ============================================================
// SHOW TAKE TEST PAGE
// ============================================================

void StudentExamWindow::showTakeTestPage()
{
    setupTakeTestPage();
    loadQuestion();
}

// ============================================================
// LOAD QUESTION
// ============================================================

void StudentExamWindow::loadQuestion()
{
    if (questions.empty())
        return;

    ExamQuestion& question =
        questions[currentQuestion];

    questionNumberLabel->setText(
        QString("Question %1 of %2")
            .arg(currentQuestion + 1)
            .arg(static_cast<int>(questions.size()))
    );

    questionLabel->setText(
        QString::fromStdString(
            question.questionText
        )
    );

    optionA->setText(
        QString("A. %1")
            .arg(
                QString::fromStdString(
                    question.options[0]
                )
            )
    );

    optionB->setText(
        QString("B. %1")
            .arg(
                QString::fromStdString(
                    question.options[1]
                )
            )
    );

    optionC->setText(
        QString("C. %1")
            .arg(
                QString::fromStdString(
                    question.options[2]
                )
            )
    );

    optionD->setText(
        QString("D. %1")
            .arg(
                QString::fromStdString(
                    question.options[3]
                )
            )
    );

    optionA->setVisible(!question.isWritten);
    optionB->setVisible(!question.isWritten);
    optionC->setVisible(!question.isWritten);
    optionD->setVisible(!question.isWritten);

    writtenAnswer->setVisible(
        question.isWritten
    );

    optionGroup->setExclusive(false);

    optionA->setChecked(false);
    optionB->setChecked(false);
    optionC->setChecked(false);
    optionD->setChecked(false);

    optionGroup->setExclusive(true);

    writtenAnswer->clear();

    StudentAnswer& answer =
        answers[currentQuestion];

    if (question.isWritten)
    {
        writtenAnswer->setPlainText(
            QString::fromStdString(
                answer.getAnswerText()
            )
        );
    }
    else
    {
        int selected =
            answer.getSelectedOptionId();

        if (selected == 1)
            optionA->setChecked(true);

        else if (selected == 2)
            optionB->setChecked(true);

        else if (selected == 3)
            optionC->setChecked(true);

        else if (selected == 4)
            optionD->setChecked(true);
    }

    previousButton->setEnabled(
        currentQuestion > 0
    );

    nextButton->setEnabled(
        currentQuestion <
        static_cast<int>(questions.size()) - 1
    );
}

// ============================================================
// SAVE CURRENT ANSWER
// ============================================================

void StudentExamWindow::saveCurrentAnswer()
{
    if (questions.empty())
        return;

    ExamQuestion& question =
        questions[currentQuestion];

    StudentAnswer& answer =
        answers[currentQuestion];

    if (question.isWritten)
    {
        answer.setAnswerText(
            writtenAnswer
                ->toPlainText()
                .toStdString()
        );
    }
    else
    {
        int selected = 0;

        if (optionA->isChecked())
            selected = 1;

        else if (optionB->isChecked())
            selected = 2;

        else if (optionC->isChecked())
            selected = 3;

        else if (optionD->isChecked())
            selected = 4;

        answer.setSelectedOption(selected);
    }
}

// ============================================================
// START EXAM
// ============================================================

void StudentExamWindow::startExam()
{
    if (questions.empty())
    {
        QMessageBox::warning(
            this,
            "Cannot Start",
            "No questions are available."
        );

        return;
    }

    QMessageBox::StandardButton reply =
        QMessageBox::question(
            this,
            "Start Test",
            "Are you eligible to start this test?",
            QMessageBox::Yes |
            QMessageBox::No
        );

    if (reply != QMessageBox::Yes)
        return;

    delete currentAttempt;

    currentAttempt =
        new TestAttempt(
            1,
            testId,
            studentId
        );

    currentQuestion = 0;

    remainingSeconds = 300;
    elapsedSeconds = 0;

    for (StudentAnswer& answer : answers)
    {
        answer.clearAnswer();
    }

    showTakeTestPage();

    updateTimer();

    examTimer->start(1000);
}

// ============================================================
// UPDATE TIMER
// ============================================================

void StudentExamWindow::updateTimer()
{
    if (remainingSeconds <= 0)
    {
        autoSubmit();
        return;
    }

    int minutes =
        remainingSeconds / 60;

    int seconds =
        remainingSeconds % 60;

    timerLabel->setText(
        QString("Time remaining: %1:%2")
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'))
    );

    remainingSeconds--;
    elapsedSeconds++;

    if (remainingSeconds < 0)
        remainingSeconds = 0;
}

// ============================================================
// SUBMIT EXAM
// ============================================================

void StudentExamWindow::submitExam()
{
    saveCurrentAnswer();

    QMessageBox::StandardButton reply =
        QMessageBox::question(
            this,
            "Submit Test",
            "Are you sure you want to submit the test?\n"
            "You cannot change your answers after submission.",
            QMessageBox::Yes |
            QMessageBox::No
        );

    if (reply != QMessageBox::Yes)
        return;

    if (currentAttempt == nullptr)
        return;

    examTimer->stop();

    currentAttempt->submit();

    calculateResult();

    showResultPage();
}

// ============================================================
// AUTO SUBMIT
// ============================================================

void StudentExamWindow::autoSubmit()
{
    saveCurrentAnswer();

    examTimer->stop();

    if (currentAttempt != nullptr)
    {
        currentAttempt->submit();
    }

    calculateResult();

    QMessageBox::information(
        this,
        "Time's Up",
        "The examination time has ended.\n"
        "Your test has been submitted automatically."
    );

    showResultPage();
}

// ============================================================
// CALCULATE RESULT
// ============================================================

void StudentExamWindow::calculateResult()
{
    delete currentResult;

    currentResult =
        new Result(1, 1);

    double maximumScore = 0.0;

    for (const ExamQuestion& question : questions)
    {
        maximumScore += question.score;
    }

    currentResult->calculate(
        answers,
        maximumScore,
        elapsedSeconds,
        currentAttempt->getSubmitTime()
    );
}

// ============================================================
// RESULT PAGE
// ============================================================

void StudentExamWindow::setupResultPage()
{
    while (QLayoutItem* item = mainLayout->takeAt(0))
    {
        if (item->widget())
        {
            delete item->widget();
        }

        delete item;
    }

    QLabel* resultTitle =
        new QLabel("Examination Result");

    resultTitle->setFont(
        QFont("Arial", 18, QFont::Bold)
    );

    scoreLabel = new QLabel();
    percentageLabel = new QLabel();
    gradeLabel = new QLabel();
    correctLabel = new QLabel();
    wrongLabel = new QLabel();
    unansweredLabel = new QLabel();
    timeTakenLabel = new QLabel();
    submittedAtLabel = new QLabel();

    backButton =
        new QPushButton(
            "Back to Available Tests"
        );

    mainLayout->addWidget(resultTitle);

    mainLayout->addSpacing(15);

    mainLayout->addWidget(scoreLabel);
    mainLayout->addWidget(percentageLabel);
    mainLayout->addWidget(gradeLabel);

    mainLayout->addWidget(correctLabel);
    mainLayout->addWidget(wrongLabel);
    mainLayout->addWidget(unansweredLabel);

    mainLayout->addWidget(timeTakenLabel);
    mainLayout->addWidget(submittedAtLabel);

    mainLayout->addSpacing(20);

    mainLayout->addWidget(backButton);

    connect(
        backButton,
        &QPushButton::clicked,
        this,
        &StudentExamWindow::showViewTestPage
    );
}

// ============================================================
// SHOW RESULT PAGE
// ============================================================

void StudentExamWindow::showResultPage()
{
    setupResultPage();

    if (currentResult == nullptr)
        return;

    scoreLabel->setText(
        QString("Score: %1 / %2")
            .arg(
                currentResult->getTotalScore(),
                0,
                'f',
                1
            )
            .arg(
                currentResult->getMaxScore(),
                0,
                'f',
                1
            )
    );

    percentageLabel->setText(
        QString("Percentage: %1%")
            .arg(
                currentResult->getPercentage(),
                0,
                'f',
                1
            )
    );

    gradeLabel->setText(
        QString("Grade: %1")
            .arg(
                QString::fromStdString(
                    currentResult->getGrade()
                )
            )
    );

    correctLabel->setText(
        QString("Correct answers: %1")
            .arg(
                currentResult->getCorrectCount()
            )
    );

    wrongLabel->setText(
        QString("Wrong answers: %1")
            .arg(
                currentResult->getWrongCount()
            )
    );

    unansweredLabel->setText(
        QString("Unanswered: %1")
            .arg(
                currentResult->getUnansweredCount()
            )
    );

    timeTakenLabel->setText(
        QString("Time taken: %1 seconds")
            .arg(
                currentResult->getTimeTaken()
            )
    );

    submittedAtLabel->setText(
        QString("Submitted at: %1")
            .arg(
                QString::fromStdString(
                    currentResult->getSubmittedAt()
                )
            )
    );
}