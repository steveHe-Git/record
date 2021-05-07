/*
 * 音乐翻译定义一套规则，我的规则是 O 表示音阶 'O 1'表示低音阶 'O 2'表示中音阶 'O 3'表示高音阶
 * 'P' 表示休止符
 * 'C D E F G A B' 表示'Do-Re-Mi-Fa-So-La-Ti', 音符长度1表示一拍，2表示二拍，0.5表示半拍，0.25表示四分之一拍
 * 'T' 表示速度 <500 慢速 >1000是快速 其余是中速
 * 所有的字母和数字都要用半角空格分开
*/
#include <iostream>
#include <memory>
#include <string>
#include <sstream>
using namespace std;

class PlayContext
{
private:
    /* data */
    string context;
public:
    PlayContext(/* args */){}
    ~PlayContext(){}
    string getContext() {
        return context;
    }

    void setContext(string val) {
        context = val;
    }
};

//表达式
class expression
{
private:
    /* data */
public:
    expression(/* args */){}
    virtual ~expression(){}
    //解释器
    void interpreter(PlayContext* context) {
        //第一种方式
        string playKey;
        double playValue;
        if (context->getContext().size() == 0) {
            return;
        } else {
            string tmp = context->getContext();
            istringstream istr(tmp);
            istr >> playKey >> playValue;
            string text;
            getline(istr, text);
            context->setContext(text);
            executable(playKey, playValue);
        }

        //第二种方式
        // if (context->getContext().size() == 0) {
        //     return;
        // } else {
        //     string playKey = context->getContext().substr(0, 1);
        //     string playText = context->getContext().substr(2);
        //     double palyValue = atof(playText.c_str());
        //     context->setContext(context->getContext().substr(context->getContext().find(" ")+1));

        //     executable(playKey, palyValue);
        // }
    }

    //解析
    virtual void executable(string key, double value) = 0;
};

//音符
class Note:public expression
{
private:
    /* data */
    string note;
    string notePai;
public:
    Note(/* args */){}
    ~Note(){}
    void executable(string key, double value){
        if (key == "C") {
            note = "Do";
        } else if (key == "D") {
            note = "Re";
        } else if (key == "E") {
            note = "Mi";
        } else if (key == "F") {
            note = "Fa";
        } else if (key == "G") {
            note = "So";
        } else if (key == "A") {
            note = "La";
        } else if (key == "B") {
            note = "Ti";
        }

        if (value == 0.25) {
            notePai = "四分之一拍";
        } else if (value == 0.5) {
            notePai = "半拍";
        } else if (value == 1) {
            notePai = "一拍";
        } else if (value == 2) {
            notePai = "二拍";
        }  else if (value == 3) {
            notePai = "三拍";
        } 

        cout << note << " ";
        cout << notePai << " ";
    }
};

//音阶
class Scale:public expression
{
private:
    /* data */
    string scale;
public:
    Scale(/* args */){}
    ~Scale(){}
    void executable(string key, double value) {
        if (value == 1) {
            scale = "低阶";
        } else if (value == 2) {
            scale = "中阶";
        } else if (value == 3) {
            scale = "高阶";
        }

        cout << scale << " ";
    }
};

//音速
class Speed:public expression
{
private:
    /* data */
    string strSpeed;
public:
    Speed(/* args */){}
    ~Speed(){}
    void executable(string key, double value) {
        if (value < 500) {
            strSpeed = "慢速";
        } else if (value > 1000) {
            strSpeed = "快速";
        } else {
            strSpeed = "中速";
        }

        cout << strSpeed << " ";
    }
};

int main () {
    auto context = new PlayContext();
    context->setContext("T 500 O 2 E 0.5 G 0.5 A 3 E 0.5 G 0.5 D 3 E 0.5 G 0.5 A 0.5 O 3 C 1 O 2 A 0.5 G 1 C 0.5 E 0.5 D 3");
    expression* exp = nullptr;
    cout << "上海滩 " <<endl;
    try{
        while (context->getContext().size() > 0) {
            string str = context->getContext().substr(0, 1);
            if (str == "T") {
                exp = new Speed();
            } else if (str == "O") {
                cout << "...." << endl;
                exp = new Scale();
            } else {
                exp = new Note();
            }

            exp->interpreter(context);
        }
        cout << endl;
    } catch (exception& e) {
        cout << e.what();
    }

    
    return 0;
}