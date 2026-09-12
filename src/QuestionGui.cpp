#include "QuestionGui.h"

#include "Course.h"
#include "Topic.h"
#include "Question.h"
#include "AnswerOption.h"

#include <windows.h>

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace fs = std::filesystem;

namespace
{
    const char *WINDOW_CLASS = "QuizQuestionManagementWindow";

    enum ControlID
    {
        ID_COURSE_LIST = 1001,
        ID_COURSE_NAME,
        ID_COURSE_DESC,
        ID_COURSE_ADD,
        ID_COURSE_UPDATE,
        ID_COURSE_DELETE,

        ID_TOPIC_LIST,
        ID_TOPIC_NAME,
        ID_TOPIC_DESC,
        ID_TOPIC_ADD,
        ID_TOPIC_UPDATE,
        ID_TOPIC_DELETE,

        ID_QUESTION_LIST,
        ID_QUESTION_CONTENT,
        ID_QUESTION_SEARCH,
        ID_QUESTION_SEARCH_BUTTON,

        ID_TOPIC_FILTER,
        ID_DIFFICULTY_FILTER,

        ID_OPTION_A,
        ID_OPTION_B,
        ID_OPTION_C,
        ID_OPTION_D,

        ID_CORRECT_COMBO,
        ID_DIFFICULTY_COMBO,

        ID_QUESTION_ADD,
        ID_QUESTION_UPDATE,
        ID_QUESTION_DELETE
    };

    struct TopicData
    {
        Topic topic;
        int courseId;
    };

    struct QuestionData
    {
        Question question;
        int topicId;
        std::vector<AnswerOption> options;
    };

    struct AppData
    {
        HWND hwnd = nullptr;

        HWND courseList = nullptr;
        HWND courseName = nullptr;
        HWND courseDesc = nullptr;

        HWND topicList = nullptr;
        HWND topicName = nullptr;
        HWND topicDesc = nullptr;

        HWND questionList = nullptr;
        HWND questionContent = nullptr;
        HWND questionSearch = nullptr;

        HWND topicFilter = nullptr;
        HWND difficultyFilter = nullptr;

        HWND optionA = nullptr;
        HWND optionB = nullptr;
        HWND optionC = nullptr;
        HWND optionD = nullptr;

        HWND correctCombo = nullptr;
        HWND difficultyCombo = nullptr;

        std::vector<Course> courses;
        std::vector<TopicData> topics;
        std::vector<QuestionData> questions;

        int selectedCourse = -1;
        int selectedTopic = -1;
        int selectedQuestion = -1;
    };

    AppData *getApp(HWND hwnd)
    {
        return reinterpret_cast<AppData *>(
            GetWindowLongPtrA(hwnd, GWLP_USERDATA));
    }

    std::string getText(HWND control)
    {
        int length = GetWindowTextLengthA(control);

        if (length <= 0)
            return "";

        std::string text(length, '\0');

        GetWindowTextA(
            control,
            &text[0],
            length + 1);

        return text;
    }

    void setText(HWND control, const std::string &text)
    {
        SetWindowTextA(control, text.c_str());
    }

    void ensureDataFolder()
    {
        fs::create_directories("data");
    }

    std::string cleanText(std::string value)
    {
        std::replace(value.begin(), value.end(), '|', ' ');
        return value;
    }

    std::string difficultyToString(DifficultyLevel difficulty)
    {
        switch (difficulty)
        {
        case DifficultyLevel::Medium:
            return "Medium";

        case DifficultyLevel::Hard:
            return "Hard";

        default:
            return "Easy";
        }
    }

    DifficultyLevel stringToDifficulty(const std::string &value)
    {
        if (value == "Medium")
            return DifficultyLevel::Medium;

        if (value == "Hard")
            return DifficultyLevel::Hard;

        return DifficultyLevel::Easy;
    }

    void saveCourses(AppData *app)
    {
        ensureDataFolder();

        std::ofstream file("data/courses.txt");

        for (const Course &course : app->courses)
        {
            file
                << course.getCourseId()
                << "|"
                << cleanText(course.getName())
                << "|"
                << cleanText(course.getDescription())
                << "\n";
        }
    }

    void saveTopics(AppData *app)
    {
        ensureDataFolder();

        std::ofstream file("data/topics.txt");

        for (const TopicData &data : app->topics)
        {
            file
                << data.topic.getTopicId()
                << "|"
                << data.courseId
                << "|"
                << cleanText(data.topic.getName())
                << "|"
                << cleanText(data.topic.getDescription())
                << "\n";
        }
    }

    void saveQuestions(AppData *app)
    {
        ensureDataFolder();

        std::ofstream questionFile("data/questions.txt");
        std::ofstream optionFile("data/answer_options.txt");

        for (const QuestionData &data : app->questions)
        {
            questionFile
                << data.question.getQuestionId()
                << "|"
                << data.topicId
                << "|"
                << difficultyToString(
                       data.question.getDifficultyLevel())
                << "|"
                << cleanText(data.question.getContent())
                << "\n";

            for (const AnswerOption &option : data.options)
            {
                optionFile
                    << data.question.getQuestionId()
                    << "|"
                    << option.getOptionId()
                    << "|"
                    << option.getIsCorrect()
                    << "|"
                    << cleanText(option.getContent())
                    << "\n";
            }
        }
    }

    void loadCourses(AppData *app)
    {
        app->courses.clear();

        std::ifstream file("data/courses.txt");

        if (!file.is_open())
            return;

        std::string line;

        while (std::getline(file, line))
        {
            if (line.empty())
                continue;

            std::stringstream ss(line);

            std::string idText;
            std::string name;
            std::string description;

            std::getline(ss, idText, '|');
            std::getline(ss, name, '|');
            std::getline(ss, description);

            try
            {
                int id = std::stoi(idText);

                app->courses.emplace_back(
                    id,
                    name,
                    description);
            }
            catch (...)
            {
            }
        }
    }

    void loadTopics(AppData *app)
    {
        app->topics.clear();

        std::ifstream file("data/topics.txt");

        if (!file.is_open())
            return;

        std::string line;

        while (std::getline(file, line))
        {
            if (line.empty())
                continue;

            std::stringstream ss(line);

            std::string idText;
            std::string courseIdText;
            std::string name;
            std::string description;

            std::getline(ss, idText, '|');
            std::getline(ss, courseIdText, '|');
            std::getline(ss, name, '|');
            std::getline(ss, description);

            try
            {
                int id = std::stoi(idText);
                int courseId = std::stoi(courseIdText);

                TopicData data{
                    Topic(id, name, description),
                    courseId};

                app->topics.push_back(data);
            }
            catch (...)
            {
            }
        }
    }

    void loadQuestions(AppData *app)
    {
        app->questions.clear();

        std::ifstream questionFile("data/questions.txt");

        if (!questionFile.is_open())
            return;

        std::string line;

        while (std::getline(questionFile, line))
        {
            if (line.empty())
                continue;

            std::stringstream ss(line);

            std::string idText;
            std::string topicIdText;
            std::string difficultyText;
            std::string content;

            std::getline(ss, idText, '|');
            std::getline(ss, topicIdText, '|');
            std::getline(ss, difficultyText, '|');
            std::getline(ss, content);

            try
            {
                int id = std::stoi(idText);
                int topicId = std::stoi(topicIdText);

                QuestionData data{
                    Question(
                        id,
                        content,
                        stringToDifficulty(difficultyText)),
                    topicId,
                    {}};

                app->questions.push_back(data);
            }
            catch (...)
            {
            }
        }

        std::ifstream optionFile("data/answer_options.txt");

        if (!optionFile.is_open())
            return;

        while (std::getline(optionFile, line))
        {
            if (line.empty())
                continue;

            std::stringstream ss(line);

            std::string questionIdText;
            std::string optionIdText;
            std::string correctText;
            std::string content;

            std::getline(ss, questionIdText, '|');
            std::getline(ss, optionIdText, '|');
            std::getline(ss, correctText, '|');
            std::getline(ss, content);

            try
            {
                int questionId = std::stoi(questionIdText);
                int optionId = std::stoi(optionIdText);
                bool correct = correctText == "1";

                for (QuestionData &question : app->questions)
                {
                    if (question.question.getQuestionId() == questionId)
                    {
                        question.options.emplace_back(
                            optionId,
                            content,
                            correct);

                        break;
                    }
                }
            }
            catch (...)
            {
            }
        }
    }

    int nextCourseId(AppData *app)
    {
        int maxId = 0;

        for (const Course &course : app->courses)
            maxId = std::max(maxId, course.getCourseId());

        return maxId + 1;
    }

    int nextTopicId(AppData *app)
    {
        int maxId = 0;

        for (const TopicData &topic : app->topics)
            maxId = std::max(maxId, topic.topic.getTopicId());

        return maxId + 1;
    }

    int nextQuestionId(AppData *app)
    {
        int maxId = 0;

        for (const QuestionData &question : app->questions)
            maxId = std::max(
                maxId,
                question.question.getQuestionId());

        return maxId + 1;
    }

    void clearList(HWND list)
    {
        SendMessageA(
            list,
            LB_RESETCONTENT,
            0,
            0);
    }

    void addListItem(HWND list, const std::string &text)
    {
        SendMessageA(
            list,
            LB_ADDSTRING,
            0,
            reinterpret_cast<LPARAM>(text.c_str()));
    }

    void refreshCourseList(AppData *app)
    {
        clearList(app->courseList);

        for (const Course &course : app->courses)
        {
            std::string text =
                std::to_string(course.getCourseId()) + " - " + course.getName();

            addListItem(
                app->courseList,
                text);
        }
    }

    void refreshTopicList(AppData *app)
    {
        clearList(app->topicList);

        for (const TopicData &data : app->topics)
        {
            std::string text =
                std::to_string(data.topic.getTopicId()) + " - " + data.topic.getName();

            addListItem(
                app->topicList,
                text);
        }
    }

    void refreshQuestionList(AppData *app)
    {
        clearList(app->questionList);

        std::string keyword =
            getText(app->questionSearch);

        std::string topicFilter =
            getText(app->topicFilter);

        std::string difficultyFilter =
            getText(app->difficultyFilter);

        for (const QuestionData &data : app->questions)
        {
            std::string content =
                data.question.getContent();

            bool keywordOK = true;
            bool topicOK = true;
            bool difficultyOK = true;

            if (!keyword.empty())
            {
                std::string lowerContent = content;
                std::string lowerKeyword = keyword;

                std::transform(
                    lowerContent.begin(),
                    lowerContent.end(),
                    lowerContent.begin(),
                    ::tolower);

                std::transform(
                    lowerKeyword.begin(),
                    lowerKeyword.end(),
                    lowerKeyword.begin(),
                    ::tolower);

                keywordOK =
                    lowerContent.find(lowerKeyword) != std::string::npos;
            }

            if (topicFilter != "All")
            {
                int topicId =
                    data.topicId;

                std::string selectedTopicId =
                    std::to_string(topicId);

                topicOK =
                    selectedTopicId == topicFilter;
            }

            if (difficultyFilter != "All")
            {
                difficultyOK =
                    difficultyToString(
                        data.question.getDifficultyLevel()) == difficultyFilter;
            }

            if (keywordOK &&
                topicOK &&
                difficultyOK)
            {
                std::string text =
                    std::to_string(
                        data.question.getQuestionId()) +
                    " - " + content;

                addListItem(
                    app->questionList,
                    text);
            }
        }
    }

    void refreshTopicFilter(AppData *app)
    {
        SendMessageA(
            app->topicFilter,
            CB_RESETCONTENT,
            0,
            0);

        SendMessageA(
            app->topicFilter,
            CB_ADDSTRING,
            0,
            reinterpret_cast<LPARAM>("All"));

        for (const TopicData &data : app->topics)
        {
            std::string id =
                std::to_string(
                    data.topic.getTopicId());

            SendMessageA(
                app->topicFilter,
                CB_ADDSTRING,
                0,
                reinterpret_cast<LPARAM>(id.c_str()));
        }

        SendMessageA(
            app->topicFilter,
            CB_SETCURSEL,
            0,
            0);
    }

    void loadCourseToForm(AppData *app)
    {
        int index =
            static_cast<int>(
                SendMessageA(
                    app->courseList,
                    LB_GETCURSEL,
                    0,
                    0));

        if (index < 0 ||
            index >= static_cast<int>(
                         app->courses.size()))
            return;

        app->selectedCourse = index;

        const Course &course =
            app->courses[index];

        setText(
            app->courseName,
            course.getName());

        setText(
            app->courseDesc,
            course.getDescription());
    }

    void loadTopicToForm(AppData *app)
    {
        int index =
            static_cast<int>(
                SendMessageA(
                    app->topicList,
                    LB_GETCURSEL,
                    0,
                    0));

        if (index < 0 ||
            index >= static_cast<int>(
                         app->topics.size()))
            return;

        app->selectedTopic = index;

        const TopicData &data =
            app->topics[index];

        setText(
            app->topicName,
            data.topic.getName());

        setText(
            app->topicDesc,
            data.topic.getDescription());
    }

    void loadQuestionToForm(AppData *app)
    {
        int listIndex =
            static_cast<int>(
                SendMessageA(
                    app->questionList,
                    LB_GETCURSEL,
                    0,
                    0));

        if (listIndex < 0)
            return;

        std::string selectedText(512, '\0');

        SendMessageA(
            app->questionList,
            LB_GETTEXT,
            listIndex,
            reinterpret_cast<LPARAM>(
                &selectedText[0]));

        selectedText.resize(
            strlen(selectedText.c_str()));

        size_t separator =
            selectedText.find(" - ");

        if (separator == std::string::npos)
            return;

        int questionId = 0;

        try
        {
            questionId =
                std::stoi(
                    selectedText.substr(
                        0,
                        separator));
        }
        catch (...)
        {
            return;
        }

        for (int i = 0;
             i < static_cast<int>(
                     app->questions.size());
             ++i)
        {
            if (app->questions[i]
                    .question
                    .getQuestionId() == questionId)
            {
                app->selectedQuestion = i;

                const QuestionData &data =
                    app->questions[i];

                setText(
                    app->questionContent,
                    data.question.getContent());

                int difficultyIndex = 0;

                switch (
                    data.question
                        .getDifficultyLevel())
                {
                case DifficultyLevel::Medium:
                    difficultyIndex = 1;
                    break;

                case DifficultyLevel::Hard:
                    difficultyIndex = 2;
                    break;

                default:
                    difficultyIndex = 0;
                    break;
                }

                SendMessageA(
                    app->difficultyCombo,
                    CB_SETCURSEL,
                    difficultyIndex,
                    0);

                for (int j = 0; j < 4; ++j)
                {
                    std::string value = "";

                    if (j < static_cast<int>(
                                data.options.size()))
                    {
                        value =
                            data.options[j]
                                .getContent();
                    }

                    HWND control = nullptr;

                    if (j == 0)
                        control = app->optionA;

                    if (j == 1)
                        control = app->optionB;

                    if (j == 2)
                        control = app->optionC;

                    if (j == 3)
                        control = app->optionD;

                    setText(control, value);
                }

                int correctIndex = 0;

                for (int j = 0;
                     j < static_cast<int>(
                             data.options.size());
                     ++j)
                {
                    if (data.options[j]
                            .getIsCorrect())
                    {
                        correctIndex = j;
                        break;
                    }
                }

                SendMessageA(
                    app->correctCombo,
                    CB_SETCURSEL,
                    correctIndex,
                    0);

                break;
            }
        }
    }

    bool questionIsUsed(int questionId)
    {
        std::ifstream file(
            "data/test_questions.txt");

        if (!file.is_open())
            return false;

        std::string line;

        while (std::getline(file, line))
        {
            std::stringstream ss(line);

            std::string testId;
            std::string storedQuestionId;

            std::getline(
                ss,
                testId,
                '|');

            std::getline(
                ss,
                storedQuestionId,
                '|');

            try
            {
                if (std::stoi(storedQuestionId) == questionId)
                {
                    return true;
                }
            }
            catch (...)
            {
            }
        }

        return false;
    }

    void addCourse(AppData *app)
    {
        std::string name =
            getText(app->courseName);

        std::string description =
            getText(app->courseDesc);

        if (name.empty())
        {
            MessageBoxA(
                app->hwnd,
                "Course name is required.",
                "Warning",
                MB_OK | MB_ICONWARNING);

            return;
        }

        Course course(
            nextCourseId(app),
            name,
            description);

        app->courses.push_back(course);

        saveCourses(app);
        refreshCourseList(app);

        setText(app->courseName, "");
        setText(app->courseDesc, "");

        MessageBoxA(
            app->hwnd,
            "Course created successfully.",
            "Course",
            MB_OK | MB_ICONINFORMATION);
    }

    void updateCourse(AppData *app)
    {
        if (app->selectedCourse < 0 ||
            app->selectedCourse >=
                static_cast<int>(
                    app->courses.size()))
        {
            MessageBoxA(
                app->hwnd,
                "Please select a course.",
                "Warning",
                MB_OK | MB_ICONWARNING);

            return;
        }

        std::string name =
            getText(app->courseName);

        if (name.empty())
        {
            MessageBoxA(
                app->hwnd,
                "Course name is required.",
                "Warning",
                MB_OK | MB_ICONWARNING);

            return;
        }

        app->courses[app->selectedCourse].setName(name);

        app->courses[app->selectedCourse].setDescription(
            getText(app->courseDesc));

        saveCourses(app);
        refreshCourseList(app);

        MessageBoxA(
            app->hwnd,
            "Course updated successfully.",
            "Course",
            MB_OK | MB_ICONINFORMATION);
    }

    void deleteCourse(AppData *app)
    {
        if (app->selectedCourse < 0 ||
            app->selectedCourse >=
                static_cast<int>(
                    app->courses.size()))
        {
            MessageBoxA(
                app->hwnd,
                "Please select a course.",
                "Warning",
                MB_OK | MB_ICONWARNING);

            return;
        }

        int courseId =
            app->courses[app->selectedCourse].getCourseId();

        for (const TopicData &topic : app->topics)
        {
            if (topic.courseId == courseId)
            {
                MessageBoxA(
                    app->hwnd,
                    "Cannot delete this course because it still has topics.",
                    "Course",
                    MB_OK | MB_ICONWARNING);

                return;
            }
        }

        app->courses.erase(
            app->courses.begin() + app->selectedCourse);

        app->selectedCourse = -1;

        saveCourses(app);
        refreshCourseList(app);

        setText(app->courseName, "");
        setText(app->courseDesc, "");

        MessageBoxA(
            app->hwnd,
            "Course deleted successfully.",
            "Course",
            MB_OK | MB_ICONINFORMATION);
    }

    void addTopic(AppData *app)
    {
        if (app->courses.empty())
        {
            MessageBoxA(
                app->hwnd,
                "Please create a course first.",
                "Topic",
                MB_OK | MB_ICONWARNING);

            return;
        }

        std::string name =
            getText(app->topicName);

        if (name.empty())
        {
            MessageBoxA(
                app->hwnd,
                "Topic name is required.",
                "Warning",
                MB_OK | MB_ICONWARNING);

            return;
        }

        int courseId =
            app->courses[0].getCourseId();

        if (app->selectedCourse >= 0)
        {
            courseId =
                app->courses[app->selectedCourse].getCourseId();
        }

        TopicData data{
            Topic(
                nextTopicId(app),
                name,
                getText(app->topicDesc)),
            courseId};

        app->topics.push_back(data);

        saveTopics(app);
        refreshTopicList(app);
        refreshTopicFilter(app);

        setText(app->topicName, "");
        setText(app->topicDesc, "");

        MessageBoxA(
            app->hwnd,
            "Topic created successfully.",
            "Topic",
            MB_OK | MB_ICONINFORMATION);
    }

    void updateTopic(AppData *app)
    {
        if (app->selectedTopic < 0 ||
            app->selectedTopic >=
                static_cast<int>(
                    app->topics.size()))
        {
            MessageBoxA(
                app->hwnd,
                "Please select a topic.",
                "Warning",
                MB_OK | MB_ICONWARNING);

            return;
        }

        std::string name =
            getText(app->topicName);

        if (name.empty())
        {
            MessageBoxA(
                app->hwnd,
                "Topic name is required.",
                "Warning",
                MB_OK | MB_ICONWARNING);

            return;
        }

        app->topics[app->selectedTopic].topic.setName(name);

        app->topics[app->selectedTopic].topic.setDescription(
            getText(app->topicDesc));

        saveTopics(app);
        refreshTopicList(app);

        MessageBoxA(
            app->hwnd,
            "Topic updated successfully.",
            "Topic",
            MB_OK | MB_ICONINFORMATION);
    }

    void deleteTopic(AppData *app)
    {
        if (app->selectedTopic < 0 ||
            app->selectedTopic >=
                static_cast<int>(
                    app->topics.size()))
        {
            MessageBoxA(
                app->hwnd,
                "Please select a topic.",
                "Warning",
                MB_OK | MB_ICONWARNING);

            return;
        }

        int topicId =
            app->topics[app->selectedTopic].topic.getTopicId();

        for (const QuestionData &question :
             app->questions)
        {
            if (question.topicId == topicId)
            {
                MessageBoxA(
                    app->hwnd,
                    "Cannot delete this topic because it still has questions.",
                    "Topic",
                    MB_OK | MB_ICONWARNING);

                return;
            }
        }

        app->topics.erase(
            app->topics.begin() + app->selectedTopic);

        app->selectedTopic = -1;

        saveTopics(app);
        refreshTopicList(app);
        refreshTopicFilter(app);

        setText(app->topicName, "");
        setText(app->topicDesc, "");

        MessageBoxA(
            app->hwnd,
            "Topic deleted successfully.",
            "Topic",
            MB_OK | MB_ICONINFORMATION);
    }

    std::vector<AnswerOption>
    createOptionsFromForm(AppData *app)
    {
        std::vector<AnswerOption> options;

        HWND controls[4] = {
            app->optionA,
            app->optionB,
            app->optionC,
            app->optionD};

        int correctIndex =
            static_cast<int>(
                SendMessageA(
                    app->correctCombo,
                    CB_GETCURSEL,
                    0,
                    0));

        for (int i = 0; i < 4; ++i)
        {
            options.emplace_back(
                i + 1,
                getText(controls[i]),
                i == correctIndex);
        }

        return options;
    }

    void addQuestion(AppData *app)
    {
        std::string content =
            getText(app->questionContent);

        if (content.empty())
        {
            MessageBoxA(
                app->hwnd,
                "Question content is required.",
                "Question",
                MB_OK | MB_ICONWARNING);

            return;
        }

        if (app->topics.empty())
        {
            MessageBoxA(
                app->hwnd,
                "Please create a topic first.",
                "Question",
                MB_OK | MB_ICONWARNING);

            return;
        }

        int topicIndex =
            static_cast<int>(
                SendMessageA(
                    app->topicFilter,
                    CB_GETCURSEL,
                    0,
                    0));

        int topicId =
            app->topics[0]
                .topic
                .getTopicId();

        if (topicIndex > 0 &&
            topicIndex <=
                static_cast<int>(
                    app->topics.size()))
        {
            topicId =
                app->topics[topicIndex - 1].topic.getTopicId();
        }
        else if (app->selectedTopic >= 0)
        {
            topicId =
                app->topics[app->selectedTopic].topic.getTopicId();
        }

        int difficultyIndex =
            static_cast<int>(
                SendMessageA(
                    app->difficultyCombo,
                    CB_GETCURSEL,
                    0,
                    0));

        DifficultyLevel difficulty =
            DifficultyLevel::Easy;

        if (difficultyIndex == 1)
            difficulty = DifficultyLevel::Medium;

        if (difficultyIndex == 2)
            difficulty = DifficultyLevel::Hard;

        QuestionData data{
            Question(
                nextQuestionId(app),
                content,
                difficulty),
            topicId,
            createOptionsFromForm(app)};

        app->questions.push_back(data);

        saveQuestions(app);
        refreshQuestionList(app);

        MessageBoxA(
            app->hwnd,
            "Question created successfully.",
            "Question Bank",
            MB_OK | MB_ICONINFORMATION);
    }

    void updateQuestion(AppData *app)
    {
        if (app->selectedQuestion < 0 ||
            app->selectedQuestion >=
                static_cast<int>(
                    app->questions.size()))
        {
            MessageBoxA(
                app->hwnd,
                "Please select a question.",
                "Question",
                MB_OK | MB_ICONWARNING);

            return;
        }

        QuestionData &data =
            app->questions[app->selectedQuestion];

        std::string content =
            getText(app->questionContent);

        if (content.empty())
        {
            MessageBoxA(
                app->hwnd,
                "Question content is required.",
                "Question",
                MB_OK | MB_ICONWARNING);

            return;
        }

        data.question.setContent(content);

        int difficultyIndex =
            static_cast<int>(
                SendMessageA(
                    app->difficultyCombo,
                    CB_GETCURSEL,
                    0,
                    0));

        DifficultyLevel difficulty =
            DifficultyLevel::Easy;

        if (difficultyIndex == 1)
            difficulty = DifficultyLevel::Medium;

        if (difficultyIndex == 2)
            difficulty = DifficultyLevel::Hard;

        data.question.setDifficultyLevel(
            difficulty);

        data.options =
            createOptionsFromForm(app);

        saveQuestions(app);
        refreshQuestionList(app);

        MessageBoxA(
            app->hwnd,
            "Question updated successfully.",
            "Question Bank",
            MB_OK | MB_ICONINFORMATION);
    }

    void deleteQuestion(AppData *app)
    {
        if (app->selectedQuestion < 0 ||
            app->selectedQuestion >=
                static_cast<int>(
                    app->questions.size()))
        {
            MessageBoxA(
                app->hwnd,
                "Please select a question.",
                "Question",
                MB_OK | MB_ICONWARNING);

            return;
        }

        int questionId =
            app->questions[app->selectedQuestion].question.getQuestionId();

        if (questionIsUsed(questionId))
        {
            MessageBoxA(
                app->hwnd,
                "This question is currently used in a test and cannot be deleted.",
                "Question Bank",
                MB_OK | MB_ICONWARNING);

            return;
        }

        app->questions.erase(
            app->questions.begin() + app->selectedQuestion);

        app->selectedQuestion = -1;

        saveQuestions(app);
        refreshQuestionList(app);

        MessageBoxA(
            app->hwnd,
            "Question deleted successfully.",
            "Question Bank",
            MB_OK | MB_ICONINFORMATION);
    }

    HWND createControl(
        DWORD style,
        const char *className,
        const char *text,
        int x,
        int y,
        int width,
        int height,
        HWND parent,
        int id)
    {
        return CreateWindowExA(
            0,
            className,
            text,
            style,
            x,
            y,
            width,
            height,
            parent,
            reinterpret_cast<HMENU>(
                static_cast<INT_PTR>(id)),
            GetModuleHandleA(nullptr),
            nullptr);
    }

    void createGuiControls(AppData *app)
    {
        HWND hwnd = app->hwnd;

        createControl(
            WS_CHILD | WS_VISIBLE,
            "STATIC",
            "COURSE MANAGEMENT",
            20, 15, 300, 25,
            hwnd, 0);

        app->courseList =
            createControl(
                WS_CHILD |
                    WS_VISIBLE |
                    WS_BORDER |
                    LBS_NOTIFY,
                "LISTBOX",
                "",
                20, 45, 300, 150,
                hwnd,
                ID_COURSE_LIST);

        createControl(
            WS_CHILD | WS_VISIBLE,
            "STATIC",
            "Name:",
            340, 45, 100, 25,
            hwnd, 0);

        app->courseName =
            createControl(
                WS_CHILD |
                    WS_VISIBLE |
                    WS_BORDER,
                "EDIT",
                "",
                440, 42, 280, 25,
                hwnd,
                ID_COURSE_NAME);

        createControl(
            WS_CHILD | WS_VISIBLE,
            "STATIC",
            "Description:",
            340, 80, 100, 25,
            hwnd, 0);

        app->courseDesc =
            createControl(
                WS_CHILD |
                    WS_VISIBLE |
                    WS_BORDER,
                "EDIT",
                "",
                440, 77, 280, 50,
                hwnd,
                ID_COURSE_DESC);

        createControl(
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            "BUTTON",
            "Add Course",
            340, 140, 115, 30,
            hwnd,
            ID_COURSE_ADD);

        createControl(
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            "BUTTON",
            "Update",
            465, 140, 100, 30,
            hwnd,
            ID_COURSE_UPDATE);

        createControl(
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            "BUTTON",
            "Delete",
            575, 140, 100, 30,
            hwnd,
            ID_COURSE_DELETE);

        createControl(
            WS_CHILD | WS_VISIBLE,
            "STATIC",
            "TOPIC MANAGEMENT",
            20, 220, 300, 25,
            hwnd, 0);

        app->topicList =
            createControl(
                WS_CHILD |
                    WS_VISIBLE |
                    WS_BORDER |
                    LBS_NOTIFY,
                "LISTBOX",
                "",
                20, 250, 300, 150,
                hwnd,
                ID_TOPIC_LIST);

        createControl(
            WS_CHILD | WS_VISIBLE,
            "STATIC",
            "Name:",
            340, 250, 100, 25,
            hwnd, 0);

        app->topicName =
            createControl(
                WS_CHILD |
                    WS_VISIBLE |
                    WS_BORDER,
                "EDIT",
                "",
                440, 247, 280, 25,
                hwnd,
                ID_TOPIC_NAME);

        createControl(
            WS_CHILD | WS_VISIBLE,
            "STATIC",
            "Description:",
            340, 285, 100, 25,
            hwnd, 0);

        app->topicDesc =
            createControl(
                WS_CHILD |
                    WS_VISIBLE |
                    WS_BORDER,
                "EDIT",
                "",
                440, 282, 280, 50,
                hwnd,
                ID_TOPIC_DESC);

        createControl(
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            "BUTTON",
            "Add Topic",
            340, 345, 115, 30,
            hwnd,
            ID_TOPIC_ADD);

        createControl(
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            "BUTTON",
            "Update",
            465, 345, 100, 30,
            hwnd,
            ID_TOPIC_UPDATE);

        createControl(
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            "BUTTON",
            "Delete",
            575, 345, 100, 30,
            hwnd,
            ID_TOPIC_DELETE);

        createControl(
            WS_CHILD | WS_VISIBLE,
            "STATIC",
            "QUESTION BANK",
            760, 15, 300, 25,
            hwnd, 0);

        app->questionSearch =
            createControl(
                WS_CHILD |
                    WS_VISIBLE |
                    WS_BORDER,
                "EDIT",
                "",
                760, 45, 230, 25,
                hwnd,
                ID_QUESTION_SEARCH);

        createControl(
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            "BUTTON",
            "Search",
            1000, 42, 90, 30,
            hwnd,
            ID_QUESTION_SEARCH_BUTTON);

        app->topicFilter =
            createControl(
                WS_CHILD |
                    WS_VISIBLE |
                    CBS_DROPDOWNLIST,
                "COMBOBOX",
                "",
                760, 80, 150, 120,
                hwnd,
                ID_TOPIC_FILTER);

        app->difficultyFilter =
            createControl(
                WS_CHILD |
                    WS_VISIBLE |
                    CBS_DROPDOWNLIST,
                "COMBOBOX",
                "",
                920, 80, 150, 120,
                hwnd,
                ID_DIFFICULTY_FILTER);

        SendMessageA(
            app->difficultyFilter,
            CB_ADDSTRING,
            0,
            reinterpret_cast<LPARAM>("All"));

        SendMessageA(
            app->difficultyFilter,
            CB_ADDSTRING,
            0,
            reinterpret_cast<LPARAM>("Easy"));

        SendMessageA(
            app->difficultyFilter,
            CB_ADDSTRING,
            0,
            reinterpret_cast<LPARAM>("Medium"));

        SendMessageA(
            app->difficultyFilter,
            CB_ADDSTRING,
            0,
            reinterpret_cast<LPARAM>("Hard"));

        SendMessageA(
            app->difficultyFilter,
            CB_SETCURSEL,
            0,
            0);

        app->questionList =
            createControl(
                WS_CHILD |
                    WS_VISIBLE |
                    WS_BORDER |
                    LBS_NOTIFY,
                "LISTBOX",
                "",
                760, 115, 330, 180,
                hwnd,
                ID_QUESTION_LIST);

        createControl(
            WS_CHILD | WS_VISIBLE,
            "STATIC",
            "Question:",
            760, 310, 100, 25,
            hwnd, 0);

        app->questionContent =
            createControl(
                WS_CHILD |
                    WS_VISIBLE |
                    WS_BORDER |
                    ES_MULTILINE,
                "EDIT",
                "",
                760, 340, 330, 55,
                hwnd,
                ID_QUESTION_CONTENT);

        createControl(
            WS_CHILD | WS_VISIBLE,
            "STATIC",
            "A:",
            760, 410, 25, 25,
            hwnd, 0);

        app->optionA =
            createControl(
                WS_CHILD |
                    WS_VISIBLE |
                    WS_BORDER,
                "EDIT",
                "",
                790, 407, 300, 25,
                hwnd,
                ID_OPTION_A);

        createControl(
            WS_CHILD | WS_VISIBLE,
            "STATIC",
            "B:",
            760, 445, 25, 25,
            hwnd, 0);

        app->optionB =
            createControl(
                WS_CHILD |
                    WS_VISIBLE |
                    WS_BORDER,
                "EDIT",
                "",
                790, 442, 300, 25,
                hwnd,
                ID_OPTION_B);

        createControl(
            WS_CHILD | WS_VISIBLE,
            "STATIC",
            "C:",
            760, 480, 25, 25,
            hwnd, 0);

        app->optionC =
            createControl(
                WS_CHILD |
                    WS_VISIBLE |
                    WS_BORDER,
                "EDIT",
                "",
                790, 477, 300, 25,
                hwnd,
                ID_OPTION_C);

        createControl(
            WS_CHILD | WS_VISIBLE,
            "STATIC",
            "D:",
            760, 515, 25, 25,
            hwnd, 0);

        app->optionD =
            createControl(
                WS_CHILD |
                    WS_VISIBLE |
                    WS_BORDER,
                "EDIT",
                "",
                790, 512, 300, 25,
                hwnd,
                ID_OPTION_D);

        createControl(
            WS_CHILD | WS_VISIBLE,
            "STATIC",
            "Correct:",
            760, 550, 70, 25,
            hwnd, 0);

        app->correctCombo =
            createControl(
                WS_CHILD |
                    WS_VISIBLE |
                    CBS_DROPDOWNLIST,
                "COMBOBOX",
                "",
                830, 547, 100, 100,
                hwnd,
                ID_CORRECT_COMBO);

        SendMessageA(
            app->correctCombo,
            CB_ADDSTRING,
            0,
            reinterpret_cast<LPARAM>("A"));

        SendMessageA(
            app->correctCombo,
            CB_ADDSTRING,
            0,
            reinterpret_cast<LPARAM>("B"));

        SendMessageA(
            app->correctCombo,
            CB_ADDSTRING,
            0,
            reinterpret_cast<LPARAM>("C"));

        SendMessageA(
            app->correctCombo,
            CB_ADDSTRING,
            0,
            reinterpret_cast<LPARAM>("D"));

        SendMessageA(
            app->correctCombo,
            CB_SETCURSEL,
            0,
            0);

        createControl(
            WS_CHILD | WS_VISIBLE,
            "STATIC",
            "Difficulty:",
            940, 550, 75, 25,
            hwnd, 0);

        app->difficultyCombo =
            createControl(
                WS_CHILD |
                    WS_VISIBLE |
                    CBS_DROPDOWNLIST,
                "COMBOBOX",
                "",
                1010, 547, 80, 100,
                hwnd,
                ID_DIFFICULTY_COMBO);

        SendMessageA(
            app->difficultyCombo,
            CB_ADDSTRING,
            0,
            reinterpret_cast<LPARAM>("Easy"));

        SendMessageA(
            app->difficultyCombo,
            CB_ADDSTRING,
            0,
            reinterpret_cast<LPARAM>("Medium"));

        SendMessageA(
            app->difficultyCombo,
            CB_ADDSTRING,
            0,
            reinterpret_cast<LPARAM>("Hard"));

        SendMessageA(
            app->difficultyCombo,
            CB_SETCURSEL,
            0,
            0);

        createControl(
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            "BUTTON",
            "Add Question",
            760, 590, 105, 30,
            hwnd,
            ID_QUESTION_ADD);

        createControl(
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            "BUTTON",
            "Update",
            875, 590, 90, 30,
            hwnd,
            ID_QUESTION_UPDATE);

        createControl(
            WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
            "BUTTON",
            "Delete",
            975, 590, 90, 30,
            hwnd,
            ID_QUESTION_DELETE);

        refreshCourseList(app);
        refreshTopicList(app);
        refreshTopicFilter(app);
        refreshQuestionList(app);
    }

    LRESULT CALLBACK WindowProc(
        HWND hwnd,
        UINT message,
        WPARAM wParam,
        LPARAM lParam)
    {
        AppData *app = getApp(hwnd);

        switch (message)
        {
        case WM_NCCREATE:
        {
            CREATESTRUCTA *create =
                reinterpret_cast<CREATESTRUCTA *>(
                    lParam);

            AppData *data =
                reinterpret_cast<AppData *>(
                    create->lpCreateParams);

            data->hwnd = hwnd;

            SetWindowLongPtrA(
                hwnd,
                GWLP_USERDATA,
                reinterpret_cast<LONG_PTR>(data));

            return TRUE;
        }

        case WM_CREATE:
        {
            app = getApp(hwnd);

            ensureDataFolder();

            loadCourses(app);
            loadTopics(app);
            loadQuestions(app);

            createGuiControls(app);

            return 0;
        }

        case WM_COMMAND:
        {
            if (!app)
                break;

            int id =
                LOWORD(wParam);

            int notification =
                HIWORD(wParam);

            if (id == ID_COURSE_LIST &&
                notification == LBN_SELCHANGE)
            {
                loadCourseToForm(app);
            }
            else if (id == ID_TOPIC_LIST &&
                     notification == LBN_SELCHANGE)
            {
                loadTopicToForm(app);
            }
            else if (id == ID_QUESTION_LIST &&
                     notification == LBN_SELCHANGE)
            {
                loadQuestionToForm(app);
            }
            else if (id == ID_COURSE_ADD)
            {
                addCourse(app);
            }
            else if (id == ID_COURSE_UPDATE)
            {
                updateCourse(app);
            }
            else if (id == ID_COURSE_DELETE)
            {
                deleteCourse(app);
            }
            else if (id == ID_TOPIC_ADD)
            {
                addTopic(app);
            }
            else if (id == ID_TOPIC_UPDATE)
            {
                updateTopic(app);
            }
            else if (id == ID_TOPIC_DELETE)
            {
                deleteTopic(app);
            }
            else if (id == ID_QUESTION_ADD)
            {
                addQuestion(app);
            }
            else if (id == ID_QUESTION_UPDATE)
            {
                updateQuestion(app);
            }
            else if (id == ID_QUESTION_DELETE)
            {
                deleteQuestion(app);
            }
            else if (
                id == ID_QUESTION_SEARCH_BUTTON)
            {
                refreshQuestionList(app);
            }
            else if (
                id == ID_TOPIC_FILTER &&
                notification == CBN_SELCHANGE)
            {
                refreshQuestionList(app);
            }
            else if (
                id == ID_DIFFICULTY_FILTER &&
                notification == CBN_SELCHANGE)
            {
                refreshQuestionList(app);
            }

            return 0;
        }

        case WM_CLOSE:
        {
            DestroyWindow(hwnd);
            return 0;
        }

        case WM_DESTROY:
        {
            if (app)
            {
                delete app;

                SetWindowLongPtrA(
                    hwnd,
                    GWLP_USERDATA,
                    0);
            }

            return 0;
        }
        }

        return DefWindowProcA(
            hwnd,
            message,
            wParam,
            lParam);
    }
}

void openQuestionGui(HWND parent)
{
    static bool registered = false;

    HINSTANCE instance =
        GetModuleHandleA(nullptr);

    if (!registered)
    {
        WNDCLASSA wc{};

        wc.lpfnWndProc =
            WindowProc;

        wc.hInstance =
            instance;

        wc.lpszClassName =
            WINDOW_CLASS;

        wc.hCursor =
            LoadCursor(
                nullptr,
                IDC_ARROW);

        wc.hbrBackground =
            reinterpret_cast<HBRUSH>(
                COLOR_WINDOW + 1);

        RegisterClassA(&wc);

        registered = true;
    }

    AppData *app =
        new AppData();

    HWND hwnd =
        CreateWindowExA(
            0,
            WINDOW_CLASS,
            "Question Bank Management",
            WS_OVERLAPPEDWINDOW,
            CW_USEDEFAULT,
            CW_USEDEFAULT,
            1150,
            700,
            parent,
            nullptr,
            instance,
            app);

    if (!hwnd)
    {
        delete app;
        return;
    }

    ShowWindow(
        hwnd,
        SW_SHOW);

    UpdateWindow(hwnd);
}