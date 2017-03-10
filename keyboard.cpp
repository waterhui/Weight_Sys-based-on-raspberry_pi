#include "keyboard.h"
#include "ui_keyboard.h"
#include <QString>
#include <QDebug>
#include <QLabel>

keyBoard::keyBoard(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::keyBoard)
{
    ui->setupUi(this);

    //按钮组重新标号
    ui->buttonGroup_3->setId(ui->pushButton_60 , 0);
    ui->buttonGroup_3->setId(ui->pushButton_63 , 1);
    ui->buttonGroup_3->setId(ui->pushButton_68 , 2);
    ui->buttonGroup_3->setId(ui->pushButton_58 , 3);
    ui->buttonGroup_3->setId(ui->pushButton_67 , 4);
    ui->buttonGroup_3->setId(ui->pushButton_56 , 5);
    ui->buttonGroup_3->setId(ui->pushButton_57 , 6);
    ui->buttonGroup_3->setId(ui->pushButton_61 , 7);
    ui->buttonGroup_3->setId(ui->pushButton_66 , 8);
    ui->buttonGroup_3->setId(ui->pushButton_65 , 9);

    for(int i = 0 ; i < 10 ; i++)
    {
        ui->buttonGroup_3->button(i)->setEnabled(false);
    }
    ui->pushButton_59->setEnabled(false);
    ui->pushButton_62->setEnabled(false);

    Cap = true ;
    CH_EN = true ;
    str = "" ;
    curPageNum = 0 ;
    connect(ui->buttonGroup_2 , SIGNAL(buttonClicked(QAbstractButton*)) , this , SLOT(enter(QAbstractButton*))) ;
    connect(ui->buttonGroup , SIGNAL(buttonClicked(QAbstractButton*)) , this , SLOT(GetText(QAbstractButton*))) ;
    connect(ui->buttonGroup_3 , SIGNAL(buttonClicked(QAbstractButton*)) , this , SLOT(printHZ(QAbstractButton*))) ;
    connect(ui->buttonGroup_4,SIGNAL(buttonClicked(QAbstractButton*)),this,SLOT(printNum(QAbstractButton*)));
    connect(ui->lineEdit,SIGNAL(textChanged(QString)),this,SLOT(toEdit()));

    QFile file(":/font/utf8.txt") ;
    file.open(QIODevice::ReadOnly) ;
    if(file.isOpen())
    {
        QString str1(file.readAll()) ;
        file.close();
        HZK = str1.split("\r\n");
        //qDebug()<<HZK ;
    }

    ui->lineEdit->installEventFilter(this);

    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setGeometry(0,300,1024,300);
    ui->lineEdit->setMaxLength(100);
    ui->lineEdit->setFocus();
}

keyBoard::~keyBoard()
{
    //qDebug() << "delete keyBoard";
    delete ui;
}

//第二个按钮组（shif , 中英文。。。。。）
void keyBoard::enter(QAbstractButton* btn)
{
    if(btn->text() == "Cap")
    {
        if(Cap == true)
        {
            btn->setStyleSheet("QPushButton{background-color:red}");
            Cap = false ;
        }
        else if(Cap == false)
        {
            btn->setStyleSheet("QPushButton{background-color: rgb(0, 85, 127)}");
            Cap = true ;
        }
    }
    else if(btn == ui->pushButton_38)
    {
        if(CH_EN == false)        //英文
         {
             btn->setText("EN");
             CH_EN = true ;

         }
         else if(CH_EN == true)
         {
              btn->setText("CH");
              CH_EN = false ;
         }
    }
    else if (btn->text() == "↑") {
        curLineEditPos = ui->lineEdit->cursorPosition();
        if (curLineEditPos > 20) {
            curLineEditPos -= 20;
            ui->lineEdit->setCursorPosition(curLineEditPos);
        } else {
            curLineEditPos = 0;
            ui->lineEdit->setCursorPosition(curLineEditPos);
        }
    } else if (btn->text() == "↓" ) {
        curLineEditPos = ui->lineEdit->cursorPosition();
        int max_pos = ui->lineEdit->text().size();
        if (curLineEditPos + 20 < max_pos) {
            curLineEditPos += 20;
            ui->lineEdit->setCursorPosition(curLineEditPos);
        } else {
            curLineEditPos = max_pos;
            ui->lineEdit->setCursorPosition(curLineEditPos);
        }

    }  else if(btn->text() == "←") {
        curLineEditPos = ui->lineEdit->cursorPosition();
        if (curLineEditPos > 0)
            ui->lineEdit->setCursorPosition(--curLineEditPos);
    } else if(btn->text() == "→") {
        int max_pos = ui->lineEdit->text().size();
        curLineEditPos = ui->lineEdit->cursorPosition();
        if (curLineEditPos <= max_pos) {
            ui->lineEdit->setCursorPosition(++curLineEditPos);
        }
    } else if( btn->text() == "?") {
        if (CH_EN) {
            ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, btn->text()));
        } else {
            ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, "？"));
        }
        ui->lineEdit->setCursorPosition(curLineEditPos);
    } else if (btn->text() == "!") {
        if (CH_EN) {
            ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, btn->text()));
        } else {
            ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, "！"));
        }
        ui->lineEdit->setCursorPosition(curLineEditPos);

    } else if (btn->text() == "(") {
        if (CH_EN) {
            ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, btn->text()));
        } else {
            ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, "（"));
        }
        ui->lineEdit->setCursorPosition(curLineEditPos);

    } else if (btn->text() == ")") {
        if (CH_EN) {
            ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, btn->text()));
        } else {
            ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, "）"));
        }
        ui->lineEdit->setCursorPosition(curLineEditPos);
    } else if (btn->text() == ":") {
        if (CH_EN) {
            ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, btn->text()));
        } else {
            ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, "："));
        }
        ui->lineEdit->setCursorPosition(curLineEditPos);
    } else if (btn->text() == "'") {
        if (CH_EN) {
            ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, btn->text()));
        } else {
            ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, "’"));
        }
        ui->lineEdit->setCursorPosition(curLineEditPos);
    } else if (btn->text() == "\"") {
        if (CH_EN) {
            ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, btn->text()));
        } else {
            ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, "“"));
        }
        ui->lineEdit->setCursorPosition(curLineEditPos);
    } else if (btn->text() == ".") {
        if (CH_EN) {
            ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, btn->text()));
        } else {
            ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, "。"));
        }
        ui->lineEdit->setCursorPosition(curLineEditPos);

    } else if (btn->text() == ",") {
        if (CH_EN) {
            ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, btn->text()));
        } else {
            ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, "，"));
        }
        ui->lineEdit->setCursorPosition(curLineEditPos);

    }
    else if(btn->text() == "BackSpace")
    {
        if(ui->pushButton_51->text().isEmpty())      //显示拼音的button为空的话
        {
            //就清lineEdit里的内容
            if (!_deleteSelect()) {
                if (curLineEditPos > 0) {
                    QString str = ui->lineEdit->text();
                    str.remove(curLineEditPos - 1,1);
                    curLineEditPos--;
                    ui->lineEdit->setText(str);
                    ui->lineEdit->setCursorPosition(curLineEditPos);
                }
            }
        }
        else                                          //不为空
        {
            QString str1 = ui->pushButton_51->text() ;
            str1.chop(1);                                   //一位一位地清除button里的内容
            this->curPageNum = 0;
            ui->pushButton_51->setText(str1);
            if(str1 == "")
            {
                for(int j = 0 ; j < 10 ; j++)             //先将上一页的清空
                {
                    ui->buttonGroup_3->button(j)->setText("");
                    ui->buttonGroup_3->button(j)->setEnabled(false);
                }
                return ;
            }
            //查找
            for(int i = 0 ; i < HZK.size() ; i++)
            {
                if(HZK.at(i).split(",").at(0) == ui->pushButton_51->text())     //比对拼音
                {
                    HZstr = HZK.at(i).split(",").at(1) ;   //获得汉字字符串
        //            qDebug()<<HZstr ;
                    for(int j = 0 ; j < 10 ; j++)             //先将上一页的清空
                    {
                        ui->buttonGroup_3->button(j)->setText("");
                    }
                    display() ;
                }
            }

        }

    }
    else if(btn->text() == "Exit")
    {
        ui->pushButton_51->setText("");
        emit Hide();
    }
    else if(btn->text() == "->")
    {
        for(int i = 0 ; i < 10 ; i++)
        {
            ui->buttonGroup_3->button(i)->setEnabled(false);
        }
        curPageNum++ ;
        ui->pushButton_62->setEnabled(true);
        //qDebug()<<curPageNum ;
        if(HZstr.size() - curPageNum *10 <= 10)       //最后一页
        {
            //qDebug()<<HZstr.size() - curPageNum *10 ;
            for(int i = 0 ; i < 10 ; i++)             //先将上一页的清空
            {
                ui->buttonGroup_3->button(i)->setText("");
            }
            for(int i = 0 ; i < HZstr.size() - curPageNum *10 ; i++)         //显示最后 一页的内容
            {
                ui->buttonGroup_3->button(i)->setText(HZstr.at(curPageNum *10 + i));
                ui->buttonGroup_3->button(i)->setEnabled(true);
            }
            ui->pushButton_59->setEnabled(false);               //失能->键
        }
        else
        {
            display() ;
        }
    }
    else if(btn->text() == "<-")
    {
        curPageNum-- ;
        ui->pushButton_59->setEnabled(true);
        //qDebug()<<curPageNum ;
        for(int i = 0 ; i < 10 ; i++)
        {
            ui->buttonGroup_3->button(i)->setEnabled(false);
        }
        if(curPageNum == 0)            //翻到第0页
        {
            for(int i = 0 ; i < 10 ; i++)     //显示第0页的内容
            {
                ui->buttonGroup_3->button(i)->setText(HZstr.at(i));
                if(ui->buttonGroup_3->button(i)->text() != "")
                {
                    ui->buttonGroup_3->button(i)->setEnabled(true); //
                }
            }
            ui->pushButton_62->setEnabled(false);      //最后失能<-
        }
        else
        {
            display() ;
        }
    }
    is_select = false;
    ui->lineEdit->setFocus();
}

//获得键盘输入 的字符串
void keyBoard::GetText(QAbstractButton* btn)
{
    if(Cap == false)                     //大写
    {
        this->_deleteSelect();
        str = btn->text() ;
        ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, str));
        ui->lineEdit->setCursorPosition(curLineEditPos);

    }
    else if(Cap == true)                //小写
    {
        str = btn->text() ;
        str= btn->text().toLower() ;       //变小写
        if(CH_EN == false)       //中文
        {
            //查找
            Pystr = ui->pushButton_51->text() + str;
            ui->pushButton_51->setText(ui->pushButton_51->text() + str);
            for(int i = 0 ; i < HZK.size() ; i++)
            {
                if(HZK.at(i).split(",").at(0) == ui->pushButton_51->text())     //比对拼音
                {
                    HZstr = HZK.at(i).split(",").at(1) ;   //获得汉字字符串
                    curPageNum = 0;
                    //qDebug()<<HZstr ;
                    display() ;
                    break;
                }
            }
        }
        else if(CH_EN == true)   //英文
        {
            this->_deleteSelect();
            ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, str));
            ui->lineEdit->setCursorPosition(curLineEditPos);
        }
    }
    ui->lineEdit->setFocus();
}


//显示中文
void keyBoard::display()
{
    //qDebug()<<curPageNum ;
    if(HZstr.size() <= 10)                  //拼音对应的汉字数<=10个
    {
        for(int i = 0 ; i < HZstr.size() ; i++)
        {
            ui->buttonGroup_3->button(i)->setText(HZstr.at(i));
            ui->buttonGroup_3->button(i)->setEnabled(true);
        }
        ui->pushButton_59->setEnabled(false);
        ui->pushButton_62->setEnabled(false);
    }
    else if(HZstr.size() > 10)               //拼音对应的汉字数>10个
    {
        if(curPageNum > 0)
        {
            ui->pushButton_62->setEnabled(true);
        }
        else
        {
           ui->pushButton_62->setEnabled(false);
        }
        ui->pushButton_59->setEnabled(true);
        for(int j = 0 ; j < 10 ; j++)
        {
            ui->buttonGroup_3->button(j)->setText(HZstr.at(curPageNum *10+j));
            ui->buttonGroup_3->button(j)->setEnabled(true);
        }
    }
    ui->lineEdit->setFocus();
}


void keyBoard::printHZ(QAbstractButton * btn)
{
    this->curPageNum = 0;
    this->_deleteSelect();
    ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, btn->text()));
    ui->lineEdit->setCursorPosition(curLineEditPos);
    ui->pushButton_51->setText("");
    for(int i = 0; i < 10; i++)
    {
        ui->buttonGroup_3->button(i)->setText("");
        ui->buttonGroup_3->button(i)->setEnabled(false);
    }
    ui->lineEdit->setFocus();
}

void keyBoard::printNum(QAbstractButton *btn)
{
    this->_deleteSelect();
    ui->lineEdit->setText(ui->lineEdit->text().insert(curLineEditPos++, btn->text()));
    ui->lineEdit->setCursorPosition(curLineEditPos);
    ui->lineEdit->setFocus();
}


void keyBoard::toEdit()
{
    emit Edit(ui->lineEdit->text());
}

void keyBoard::setLeText(QString s)
{
    ui->lineEdit->setText(s);
    curLineEditPos = ui->lineEdit->text().size();
    ui->lineEdit->setFocus();
}

void keyBoard::SetPwdMode()
{
    ui->lineEdit->setEchoMode(QLineEdit::Password);
}

void keyBoard::SetNormalMode()
{
    ui->lineEdit->setEchoMode(QLineEdit::Normal);
}


bool keyBoard::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->lineEdit) {
        if (event->type() == QEvent::FocusOut) {
            this->curLineEditPos = ui->lineEdit->cursorPosition();
            if (ui->lineEdit->hasSelectedText()) {
                is_select = true;
                select_len = ui->lineEdit->selectedText().size();
                select_start_pos  = ui->lineEdit->selectionStart();
            }
            return false;
        }
        return false;
    }
    return false;
}

//删除文本框中多选的字符串
bool keyBoard::_deleteSelect()
{
    if (is_select) {
        QString str = ui->lineEdit->text().remove(select_start_pos, select_len);
        ui->lineEdit->setText(str);
        if (curLineEditPos > select_len)
            curLineEditPos -= select_len;
        is_select = false;
        ui->lineEdit->setCursorPosition(curLineEditPos);
        return true;
    }
    return false;
}

/*
//鼠标按下事件
void keyBoard::mousePressEvent(QMouseEvent *event)
{
    mousePoint = event->pos();    //鼠标相对于窗体的位置
    isMousePressed = true;        //鼠标按下
    event->accept();
}

//窗体拖动事件
void keyBoard::mouseMoveEvent(QMouseEvent *event)
{
    //若鼠标左键被按下
    if(isMousePressed == true)
    {
        //鼠标相对于屏幕的位置
        QPoint curMousePoint = event->globalPos() - mousePoint;
        //移动主窗体位置
        move(curMousePoint);
    }
    event->accept();
}

//鼠标释放事件
void keyBoard::mouseReleaseEvent(QMouseEvent *event)
{
    //鼠标未按下
    isMousePressed = false;
    event->accept();
}
*/
