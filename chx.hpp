#ifndef _CHX_HPP_
#define _CHX_HPP_

#include <iostream>
#include <string>
#include <fstream>
#include <ctime>
#include <termio.h>
using namespace std;

bool if_file_exists(string filename);

string encrypt(string str);

int scanKeyboard();

void PAUSE();

class Config
{
    public:
        int users_num;
        int passages_num;
        string config_file_name;

        Config(string filename);
        void get_config();
        void set_config();
        ~Config();
};

class User
{
    protected:
        string passwd;
    public:
        string uuid;
        string username;
        void setpasswd(string tmp_pd);
        void change_passwd();
        void show();
        void save();
        void reg(Config &config);
        bool login();
};

class Passage: public User
{
    private:
        string date;
        string path;
        string title;
        string text;
    public:
        void show();
        void write(Config &config, User &user, int flag);
        void edit(Config &config, User &user, int num);
        void read(Config &config, User &user);
        void save();
        void comment(int num, User &user);
        void get_comment(int num);
};

#endif