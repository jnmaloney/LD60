#pragma once

#include <vector>
#include <string>

class Game
{
public:

    void Frame();
    void ShowTitle();
    void ShowInfo();
    void Intro();
    void ChooseActivity();
    void MakeGame();
    void PublishSequence();
    void MonthRent();
    void EndSequence();
    void MakePart();
    void ResearchTech();
    void TheMarket();
    void YearIncrease();

    void Show(const char* msg);
    void MessageTime();

    int cash_money = 3000;
    bool show_title = true;
    bool show_intro = true;
    bool end_sequence = false;
    int month = 1;
    int year = 1981;
    bool publishing = false;

    int M_Lv = 1;
    int A_Lv = 1;
    int L_Lv = 1;
    int G_Lv = 1;

    int compression_Lv = 1;

    struct Part
    {
        int category;
        int qualifier_type;
        int qualifier_value;
        int second_qualifier_type;
        int second_qualifier_value;
        int size_in_kb;
        bool selected = false;
    };
    std::vector<Part> parts;
    std::vector<Part> selected_parts;

    float hard_modifier = 1.0f;

    int choice_screen = 0;

    std::string the_message;
    int messageTimer = 0;    
};

