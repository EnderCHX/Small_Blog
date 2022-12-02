#include "chx.hpp"

Config config("config");
User user;
Passage passage;

void ui2();

void ui()
{
    char choise;
    while(true)
    {
        cout << "\033[33m\033[1m\033[41m|-------------------------------|" <<endl;
        cout << "| Welcome to Small_Blog system! |" <<endl;
        cout << "|------Input Num to Choose------|" <<endl;
        cout << "|------------1.Login------------|" <<endl;
        cout << "|----------2.Register-----------|" <<endl;
        cout << "|------------q.Exit-------------|" <<endl;
        cout << "|-------------------------------|\033[0m" <<endl;
        choise = scanKeyboard();
        system("clear");
        if(choise == '1')
        {
            cout << "\033[34m\033[1m\033[47m";
            if(user.login())
            {
                cout << "\033[0m";
                PAUSE();
                ui2();
                break;
            }
            else
            {
                PAUSE();
                cout << "\033[0m";
                continue;
            }
        }else if(choise == '2')
        {
            cout << "\033[34m\033[1m\033[47m";
            user.reg(config);
            cout << "\033[0m";
            PAUSE();
            continue;
        }else if(choise == 'q')
        {
            exit(0);
        }else
        {
            continue;
        }
    }
}

void ui2()
{
    char choise;
    while(true)
    {
        cout << "\033[33m\033[1m\033[41m|-------------------------------|" <<endl;
        cout << "|------Input Num to Choose------|" <<endl;
        cout << "|---------1.Write Blog----------|" <<endl;
        cout << "|---------2.Read Blog-----------|" <<endl;
        cout << "|-------3.Change Password-------|" <<endl;
        cout << "|------------q.Exit-------------|" <<endl;
        cout << "|-------------------------------|\033[0m" <<endl;
        choise = scanKeyboard();
        system("clear");
        if(choise == '1')
        {
            passage.write(config, user, 0);
            PAUSE();
        }else if(choise == '2')
        {
            passage.read(config,user);
            PAUSE();
        }else if(choise == '3')
        {
            cout << "\033[34m\033[1m\033[47m";
            user.change_passwd();
            cout << "\033[0m";
            PAUSE();
        }else if(choise == 'q')
        {
            exit(0);
        }
        else
        {
            continue;
        }
    }
}

int main()
{
    system("clear");
    ui();
    return 0;
}
