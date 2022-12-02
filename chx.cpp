#include "chx.hpp"

bool if_file_exists(string filename)
{
    ifstream file(filename);
    bool res = file.good();
    file.close();
    return res;
}

string encrypt(string str)
{
    for(int i = 0; i < str.length(); i++)
    {
        str[i] += 6;
    }
    return str;
}

int scanKeyboard()
{
    int input;
    struct termios new_settings;
    struct termios stored_settings;
    tcgetattr(0,&stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    tcgetattr(0,&stored_settings);
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0,TCSANOW,&new_settings);
      
    input = getchar();
      
    tcsetattr(0,TCSANOW,&stored_settings);
    return input;
}

void PAUSE()
{
    cout << "\033[33m\033[1m\033[41m按回车继续\033[0m" << endl;
    cin.get();
    cin.get();
    system("clear");
}

void User::setpasswd(string tmp_pd)
{
    tmp_pd = encrypt(tmp_pd);
    this->passwd = tmp_pd;
}

void User::change_passwd()
{
    string tmp_passwd, tmp_passwd2;
    cout << "\033[34m\033[1m\033[47m输入旧密码：";
    cout << "\033[8m";
    cin >> tmp_passwd;
    cout << "\033[0m";
    while(encrypt(tmp_passwd) != this->passwd)
    {
        cout << "\033[34m\033[1m\033[47m旧密码错误，请重新输入：";
        cout << "\033[8m";
        cin >> tmp_passwd;
        cout << "\033[0m";
    }
    cout << "\033[34m\033[1m\033[47m输入新密码:";
    cout << "\033[8m";
    cin >> tmp_passwd;
    cout << "\033[0m";
    while(tmp_passwd.size() < 8){cout << "\033[34m\033[1m\033[47m密码少于8位请重新输入:"; cout << "\033[8m"; cin >> tmp_passwd; cout << "\033[0m";}
    cout << "\033[34m\033[1m\033[47m再次输入：";
    cout << "\033[8m";
    cin >> tmp_passwd2;
    cout << "\033[0m";
    while(tmp_passwd != tmp_passwd2)
    {
        cout << "\033[34m\033[1m\033[47m两次输入不一致，请重新输入：";
        cout << "\033[8m";
        cin >> tmp_passwd;
        cout << "\033[0m";
        while(tmp_passwd.size() < 8){cout << "\033[34m\033[1m\033[47m密码少于8位请重新输入:"; cout << "\033[8m";cin >> tmp_passwd;cout << "\033[0m";}
        cout << "\033[34m\033[1m\033[47m再次输入：";
        cout << "\033[8m";
        cin >> tmp_passwd2;
        cout << "\033[0m";
    }
    this->setpasswd(tmp_passwd);
    cout << "\033[34m\033[1m\033[47m修改成功！\033[0m" << endl;
    this->save();
}

void User::show()
{
    cout << this->uuid << endl << this->username << endl << this->passwd << endl;
}

void User::save()
{
    ofstream file("./users/" + this->username);
    file << this->uuid << endl << this->username << endl << this->passwd << endl;
    file.close();
}

void User::reg(Config &config)
{
    cout << "\033[34m\033[1m\033[47m输入用户名:";
    cin >> this->username;
    while(if_file_exists("./users/"+this->username))
    {
        cout << "用户已经存在,请重新输入:\033[0m";
        cin >> this->username;
    }
    string tmp1, tmp2;
    tmp2 = "\n";
    while(tmp1 != tmp2)
    {
        cout << "\033[34m\033[1m\033[47m输入密码(不小于8位):";
        while(tmp1.size() < 8){cout << "\033[8m";cin >> tmp1;cout << "\033[0m";if(tmp1.size() < 8) cout << "\033[34m\033[1m\033[47m密码少于8位请重新输入:";}
        cout << "\033[34m\033[1m\033[47m再次输入密码:";
        cout << "\033[8m";
        cin >> tmp2;
        cout << "\033[0m";
        if(tmp1 != tmp2)
        {
            cout << "\033[34m\033[1m\033[47m两次输入不一致,请重新输入!" << endl;
            tmp1 = '\0';
        }
    }
    cout << "\033[34m\033[1m\033[47m注册成功\033[0m" << endl;
    config.users_num++;
    config.set_config();
    this->uuid = to_string(config.users_num);
    this->setpasswd(tmp1);
    this->save();
}

bool User::login()
{
    string tmp_username;
    cout << "输入用户名:";
    cin >> tmp_username;
    string filename = "./users/" + tmp_username;
    if(if_file_exists(filename))
    {
        ifstream file(filename);
        file >> this->uuid;
        file >> this->username;
        file >> this->passwd;
        file.close();
        cout << "输入密码：";
        string tmp_passwd;
        cout << "\033[8m";
        cin >> tmp_passwd;
        cout << "\033[0m";
        if(encrypt(tmp_passwd) == this->passwd)
        {
            cout << "\033[34m\033[1m\033[47m登陆成功！\033[0m\n";
            return true;
        }
        else
        {
            cout << "\033[34m\033[1m\033[47m密码错误！\n\033[0m";
            return false;
        }
    }
    else
    {
        cout << "\033[34m\033[1m\033[47m用户不存在!\n\033[0m" << endl;
        return false;
    }
}

Config::Config(string filename)
{
    this->config_file_name = filename;
    this->get_config();
}

Config::~Config()
{
    this->set_config();
}

void Config::get_config()
{
    if(if_file_exists(config_file_name))
    {
        ifstream file(config_file_name);
        file >> users_num >> passages_num;
        file.close();
    }
    else //初始化0
    {
        users_num = 100000000;
        passages_num = 0;
        system("mkdir users");
        system("mkdir passages");
        cout << config_file_name << endl;
        this->set_config();
    }
}

void Config::set_config()
{
    ofstream file(config_file_name);
    file << users_num << endl << passages_num << endl;
    file.close();
}

void Passage::read(Config &config, User &user)
{
    string root_dir = "./passages/";
    int i;
    cout << "\033[33m\033[1m\033[45m";
    cout << "|--Num---Title-------Author-----------Date----|" <<endl;
    for(i = 1; i <= config.passages_num; i++)
    {
        if(if_file_exists(root_dir + to_string(i)))
        {
            ifstream file(root_dir + to_string(i));
            getline(file, this->uuid);
            getline(file, this->username);
            getline(file, this->date);
            getline(file, this->title);
            file.close();
            cout << "|-" << i << "---" << this->title << "---" << this->username << "---" << this->date + "-|"<< endl;
        }
    }
    cout << "|---------------------------------------------|" <<endl;
    cout << "\033[0m";
    cout << "\033[34m\033[1m\033[47m输入序号查看文章(输入q退出)：\033[0m";
    char choise;
    int num;
    cin >> choise;
    num = choise - 48;
    if(choise != 'q')
    {
        while(num > config.passages_num)
        {
            cout << "\033[33m文章不存在，请重新输入：\033[0m";
            cin >> choise;
            num = choise -48;
        }
        this->path = root_dir + choise;
        ifstream file(this->path);
        getline(file, this->uuid);
        getline(file, this->username);
        getline(file, this->date);
        getline(file, this->title);
        cout << "\033[1m\033[30m\033[47m";
        cout << "    " << this->title << endl;
        cout << this->username << "  " << this->date << endl;
        string line;
        while(getline(file,line))
        {
            cout << line << endl;
        }
        cout << "\033[0m\033[34m\033[1m\033[47m" << endl;
        file.close();
        this->get_comment(num);
        while(true)
        {
            cout << "\033[0m\033[33m\033[1m\033[41m输入q退出，输入i编辑（仅作者），输入c发表评论：\033[0m";
            cout << "\033[34m\033[1m\033[47m";
            cin >> choise;
            if(choise == 'q')
            {
                break;
            }
            else if(choise == 'c')
            {
                this->comment(num, user);
            }
            else if(choise == 'i')
            {
                system("clear");
                this->path = root_dir + to_string(num);
                this->edit(config, user, num);
                break;
            }
            cout << "\033[0m";
        }
    }
}

void Passage::edit(Config &config, User &user, int num)
{
    if(user.username == this->username)
    {
        char choise;
        string root_dir = "./passages/";
        string dir1, dir2;
        while(true)
        {
            cout << "输入q退出，输入d删除文章，输入i重新编辑：";
            cin >> choise;
            if(choise == 'q')
            {
                break;
            }else if(choise == 'd')
            {
                dir1 = this->path;
                dir2 = root_dir + to_string(num) + ".comment";
                remove(dir1.c_str());
                config.passages_num--;
                cout << this->path << endl;
                for(int i = num + 1; i <= config.passages_num; i++)
                {
                    dir1 = root_dir + to_string(i);
                    dir2 = root_dir + to_string(i-1);
                    rename(dir1.c_str(), dir2.c_str());
                }
                cout << "删除成功！" << endl;
                break;
            }else if(choise == 'i')
            {
                this->write(config, user, num);
                break;
            }
        }
    }
    else
    {
        cout << "仅文章作者可编辑!" << endl;
        return;
    }
}

void Passage::write(Config &config, User &user, int flag)
{
    time_t now = time(0);
    this->date = ctime(&now);
    this->username = user.username;
    this->uuid = user.uuid;
    if(flag)
    {
        this->path = "./passages/" + to_string(flag);
    }
    else
    {
        this->path = "./passages/" + to_string(config.passages_num + 1);
        this->text = "";
    }
    string line;
    cout << "\033[34m\033[1m\033[47mInput the title of your passage:" ;
    cin >> this->title;
    cout << "\033[33m\033[1m\033[41m#################################" << endl;
    cout << "#Edit passage. Input q at start #" << endl;
    cout << "#of each line to quite without  #" << endl;
    cout << "#saving the passage. And input  #" << endl;
    cout << "#wq to quite and save!          #" << endl;
    cout << "#################################\033[34m\033[1m\033[47m" << endl;
    while(true)
    {
        getline(cin, line);
        if(line[0] == 'q' && line[1] == '\0')
        {
            cout << "退出编辑（不保存）！\033[0m" << endl;
            break;
        }
        else if(line[0] == 'w' && line[1] == 'q' && line[2] == '\0')
        {
            this->save();
            config.passages_num++;
            cout << "退出编辑（已保存）！\033[0m" << endl;
            break;
        }
        else
        {
            this->text += line + '\n';
            continue;
        }
    }
}

void Passage::save()
{
    ofstream file(this->path);
    file << this->uuid << endl;
    file << this->username << endl;
    file << this->date;
    file << this->title;
    file << this->text;
    file.close();
}

void Passage::comment(int num, User &user)
{
    string filename = "./passages/" + to_string(num) + ".comment";
    string c_username, c_date, c_text;
    if(if_file_exists(filename))
    {
        ofstream file(filename, ios::app);
        char text[400];
        cout << "请评论（不超过400字符,输入q退出）：";
        cin >> text;
        if(text[0] == 'q' && text[1] == '\0')
        {}
        else
        {
            time_t now = time(0);
            file << user.username << endl
                 << ctime(&now)
                 << text << endl;
            file.close();
        }
        get_comment(num);
    }
    else
    {
        cout << "暂无评论！" << endl;
        ofstream file(filename, ios::app);
        char text[400];
        cout << "请评论（不超过400字符,输入q退出）：";
        cin >> text;
        if(text[0] == 'q' && text[1] == '\0')
        {}
        else
        {
            time_t now = time(0);
            file << user.username << endl
                 << ctime(&now)
                 << text << endl;
            file.close();
        }
    }
}

void Passage::get_comment(int num)
{
    string filename = "./passages/" + to_string(num) + ".comment";
    string c_username, c_date, c_text;
    cout << "评论区：" << endl;
    if(if_file_exists(filename))
    {
        int count = 0;
        ifstream file(filename);
        while(!file.eof())
        {
            if(count % 3 == 0 && count != 0) cout << c_username+"(" + c_date + ")" + ":" + c_text << endl;
            getline(file,c_username);count++;
            getline(file,c_date);count++;
            getline(file,c_text);count++;
        }
        file.close();
    }
    else
    {
        cout << "暂无评论！" << endl;
    }
}