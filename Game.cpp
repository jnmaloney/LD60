#include "Game.h"
#include "imgui.h"
#include <cstdlib>
#include <ctime>
#include <initializer_list>
#include <iostream>

void Game::Frame()
{
    // Render imgui

    // state

    if (show_title)
    {
        ShowTitle();
        return;
    }


    if (show_intro)
    {
        Intro();
        return;
    }
    else if (messageTimer > 0)
    {
        MessageTime();
    }
    else if (end_sequence)
    {
        std::string msg;
        msg = "You went broke on month " + std::to_string(month) + " of year " + std::to_string(year);
        ImGui::TextWrapped("%s", msg.c_str());
        ImGui::TextUnformatted(" ");
        ImGui::Separator();
        ImGui::TextUnformatted(" ");
        if (ImGui::Button("> Try Again"))
            show_intro = 1;
    }
    else
    {
        if (choice_screen == 0)
            ShowInfo();

        if (choice_screen == 0)
            ChooseActivity();
        else if (choice_screen == 1)
            MakeGame();
        else if (choice_screen == 2)
            MakePart();
        else if (choice_screen == 3)
            ResearchTech();                        
    }
}

void Game::ShowTitle()
{
    ImGui::Text("LD60");
    ImGui::Text("FloppyD:");
    //ImGui::Image
    ImGui::TextUnformatted("");
    if (ImGui::Button("> Start Game"))
    {
        show_title = false;
    }
}

void Game::ShowInfo()
{
    // Month
    // Year

    ImGui::Text("Month: %02d", month);
    ImGui::Text("Year: %d", year);

    ImGui::Text("Cash $%d", cash_money);
    ImGui::TextUnformatted("Rent $200");

    // Capability?
    ImGui::Text("M Lv: %d", M_Lv);
    ImGui::Text("A Lv: %d", A_Lv);
    ImGui::Text("L Lv: %d", L_Lv);
    ImGui::Text("G Lv: %d", G_Lv);

    // Market info
    ImGui::Separator();
}

void Game::Intro()
{
    // Show intro message
    // Continue.

    ImGui::TextUnformatted("");
    ImGui::TextUnformatted("You are making games in your tiny office.");
    ImGui::TextUnformatted("Create and develop,");
    ImGui::TextUnformatted("then ship it on a floppy disk.");
    ImGui::TextUnformatted("");

    if (ImGui::Button("> Continue"))
    {
        show_intro = false;
    }
}

void Game::ChooseActivity()
{
    // Choose:
    // Make GAme
    // Make part
    // Improve and develop tech research

    ImGui::Text("Choose an activity:");
    if (ImGui::Button("> Ship Game"))
    {
        choice_screen = 1;
    }

    if (parts.size() < 100)
    if (ImGui::Button("> Make Part"))
    {
        choice_screen = 2;
    }

    if (ImGui::Button("> Tech Research"))
    {
        choice_screen = 3;
    }
}

void Game::MakeGame()
{
    // Maxi game loop
    // Look at you parts
    // Show the floppy disk
    // Add parts, which adds/ deducts free space
    // Remove parts
    // Publish...

    int M_score = 0;
    int A_score = 0;
    int L_score = 0;
    int G_score = 0;
    int total_size = 0;
    int total_value = 0;

    int counter = 0;
    for (auto& i : parts)    
    {
        ++counter;
        std::string customname = "##chack_" + std::to_string(counter);
        ImGui::Checkbox(customname.c_str(), &i.selected);        
        ImGui::SameLine();
        ImGui::Text("Part ");        
        ImGui::SameLine();
        if (i.category == 0) ImGui::TextUnformatted("Mechanic");
        if (i.category == 1) ImGui::TextUnformatted("Audio");
        if (i.category == 2) ImGui::TextUnformatted("Level");
        if (i.category == 3) ImGui::TextUnformatted("Graphic");
        ImGui::SameLine();

        if (i.qualifier_type == 0) ImGui::TextUnformatted("(Cute)");
        if (i.qualifier_type == 1) ImGui::TextUnformatted("(Spooky)");
        if (i.qualifier_type == 2) ImGui::TextUnformatted("(Challenging)");
        if (i.qualifier_type == 3) ImGui::TextUnformatted("(Blockbuster)");
        ImGui::SameLine();

        ImGui::Text("%d", i.qualifier_value);

        if (i.selected)
        {
            if (i.category == 0) M_score += i.qualifier_value;
            if (i.category == 1) A_score += i.qualifier_value;
            if (i.category == 2) L_score += i.qualifier_value;
            if (i.category == 3) G_score += i.qualifier_value;
            total_size += i.size_in_kb;
            total_value += i.qualifier_value;
        }
    }

    bool valid = false;
    if (M_score > 0 && A_score > 0 && L_score > 0 && G_score > 0 && total_size < 512)
    {
        valid = true;
    }

    ImGui::Text("Value: %d", total_value);
    ImGui::Text("Size: %d / 512 kB", total_size);
    ImGui::ProgressBar((float)total_size / 512.0, ImVec2(0.0f, 0.0f));


    if (valid)
    {
        if (ImGui::Button("> Ship it"))
        {
            //publishing = true;
            int first_type = -1;
            bool mixed = false;
            for (auto& i : parts)    
            {
                if (i.selected)
                {
                    if (first_type == -1)
                    {
                        first_type = i.qualifier_type;
                    }
                    else
                    {
                        if (i.qualifier_type != first_type)
                            mixed = true;
                    }
                }

            }



            int earn_value = 4000;

            int bonus_for_matching_qual = 0;
            if (!mixed) bonus_for_matching_qual = 300;

            int reduction_from_difficulty = 1000 * hard_modifier;
            if (reduction_from_difficulty > 4000) reduction_from_difficulty = 4000;

            int bonus_for_value = 200 * total_value;

            int little_bonus = 100 * (std::rand() & 5);

            int total_earn_value = earn_value + bonus_for_matching_qual - reduction_from_difficulty + bonus_for_value + little_bonus;

            cash_money += total_earn_value;

            std::erase_if(parts, [](Part x) { return x.selected; });

            std::string info = "Game Shipped! You made $" + std::to_string(total_earn_value);
            Show(info.c_str());
            MonthRent();
        }
    }
    else
    {
        ImGui::TextUnformatted("Ship it (add parts)");
    }

    if (ImGui::Button("> Go Back"))
    {
        choice_screen = 0;
    }    
}

void Game::PublishSequence()
{
    // Animate some publishing
    // Floppy gets released
    // Some feedback
    // Money income
    // 10 * rent + bonuses ???

    publishing = false;
}

void Game::MonthRent()
{
    // Update time

    // Update money

    // End sequence?

    month += 1;
    if (month == 13)
        YearIncrease();

    if (cash_money < 200)
    {
        end_sequence = true;
        return;
    }

    cash_money -= 200;

}

void Game::EndSequence()
{
    // Game over

    // Show stats

    // Show a try again message

    ImGui::TextUnformatted("You're broke and can't pay rent.");

    if (ImGui::Button("Try Again"))
    {
        end_sequence = false;
        show_intro = true;
    }
}

void Game::MakePart()
{
    // Mini game loop

    // Choose category
    // Choose a modifier
    // Choose a qualifier type or *

    // Create a part according to skill
    // More skill more qualifier points
    // Less skill might get ? qualifiers (negative) or wrong qualifiers
    // More skill might reduce the part size in kB

    // Add part to the library

    ImGui::TextUnformatted("Which Part?");
    static int e = 0;
    static int f = 0;
    static int g = 0;
    ImGui::RadioButton("Gameplay Mechanic", &e, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Music and Audio", &e, 1);
    ImGui::RadioButton("Level Design", &e, 2);
    ImGui::SameLine();
    ImGui::RadioButton("Graphics", &e, 3);

    ImGui::Separator();

    ImGui::TextUnformatted("Modifier?");
    ImGui::RadioButton("Minimalist", &f, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Regular", &f, 1);
    ImGui::SameLine();
    ImGui::RadioButton("Elaborate", &f, 2);

    ImGui::Separator();

    ImGui::TextUnformatted("Qualifier?");
    ImGui::RadioButton("Cute", &g, 0);
    ImGui::SameLine();
    ImGui::RadioButton("Spooky", &g, 1);
    ImGui::SameLine();
    ImGui::RadioButton("Challenging", &g, 2);
    ImGui::RadioButton("Blockbuster", &g, 3);
    ImGui::SameLine();
    ImGui::RadioButton("* (Wild)", &g, 4);

    ImGui::Separator();

    if (ImGui::Button("> Create part..."))
    {
        // Create part

        Part part;
        int q = g;

        int skill = 0;
        if (e == 0) skill = M_Lv;
        if (e == 1) skill = A_Lv;
        if (e == 2) skill = L_Lv;
        if (e == 3) skill = G_Lv;

        std::srand(std::time({})); // use current time as seed for random generator
        float random_value1 = (float)std::rand() / (float)RAND_MAX;
        float random_value2 = (float)std::rand() / (float)RAND_MAX;

        float chance_of_wrong_qualifier = 0.5f - 0.10f * skill;
        float qualifier_increase_modifier = 0.5f * skill;

        if (f == 0)
        {
            chance_of_wrong_qualifier *= 1.25;
            qualifier_increase_modifier *= 0.75;
        }
        if (f == 2)
        {
            chance_of_wrong_qualifier *= 1.25;
            qualifier_increase_modifier *= 0.75;
        }

        if (g == 4) // Wild
        {
            //chance_of_wrong_qualifier *= 0.5;
            qualifier_increase_modifier *= 1.5;
            q = std::rand() % 4;
        }
        else if (random_value1 < chance_of_wrong_qualifier)
        {
            q = std::rand() % 4;
            qualifier_increase_modifier = 0;
        }

        int quality = 1;
        while (random_value2 < qualifier_increase_modifier && quality < 4)
        {
            qualifier_increase_modifier *= 0.5f;
            quality += 1;
        }

        int size_in_kb = 140;
        // reduce based on skill
        int skill_reduction = 0;
        for (int i = 1; i < skill; ++i)
        {
            skill_reduction += -(std::rand() % 25);
        }
        // increase with mods
        int modifier = 0;
        if (f == 0)
        {
            modifier = -(std::rand() % 25);
        }
        if (f == 2)
        {
            modifier = std::rand() % 25;
        }

        int just_random = std::rand() % 52;
        for (int i = 1; i < compression_Lv; ++i)
        {
            if (size_in_kb > 50)
                size_in_kb -= std::rand() % 42;
        }

        part.category = e;
        part.qualifier_type = q;
        part.qualifier_value = quality;
        part.size_in_kb = size_in_kb + skill_reduction + modifier + just_random;

        parts.push_back(part);

        std::string strtype = "";
        if (q == 0) strtype = "Cute";
        if (q == 1) strtype = "Spooky";
        if (q == 2) strtype = "Challenging";
        if (q == 3) strtype = "Blockbuster";        
        std::string strquality = std::to_string(quality);
        std::string strsize = std::to_string(part.size_in_kb);
        std::string text = "Created part with type " + strtype + ", quality " + strquality + " and size " + strsize + " kB.";
        Show(text.c_str());
        MonthRent();
    }

    //parts.push_back();
    if (ImGui::Button("> Go Back"))
    {
        choice_screen = 0;
    }      
}

void Game::ResearchTech()
{
    // Choose an upgrade

    // Compression i, ii, iii, iv
    // Reduce sizes
    if (compression_Lv < 4)
    if (ImGui::Button("> Compression"))
    {
        compression_Lv += 1;
        Show("Compression Skill Increased!");
        MonthRent();
    }

    // Skill in categories
    if (ImGui::Button("> Skill Game Mechanic"))
    {
        M_Lv += 1;
        Show("Game Mechanic Skill Increased!");
        MonthRent();
    } 

    if (ImGui::Button("> Skill Music and Audio"))
    {
        A_Lv += 1;
        Show("Level Design Skill Increased!");
        MonthRent();
    }

    if (ImGui::Button("> Skill Level Design"))
    {
        L_Lv += 1;
        Show("Level Design Skill Increased!");
        MonthRent();
    }

    if (ImGui::Button("> Skill Graphics"))
    {
        G_Lv += 1;
        Show("Level Design Skill Increased!");
        MonthRent();
    }

    // Market research

    if (ImGui::Button("> Go Back"))
    {
        choice_screen = 0;
    }      
}

void Game::TheMarket()
{
    // Likes / dislikes
    // Cute, Spooky, Challenge, Blockbuster

    // See one bad/indifferent
    // See one of likes
    // See two likes
}

void Game::YearIncrease()
{
    // Next year
    // Demand gets a little bit harder

    year += 1;
    month = 1;
    hard_modifier *= 1.08f;
}

 void Game::Show(const char* msg)
 {
    // Show the message
    the_message = msg;
    messageTimer = 120;
 }

 void Game::MessageTime()
 {
    ImGui::TextWrapped("%s", the_message.c_str());
    messageTimer -=1 ;
    if (messageTimer <= 0)
    {
        choice_screen = 0;
    }
 }
