#include "manage_user_info.h"
#include "ui_manage_user_info.h"

#include <QGraphicsDropShadowEffect>

Manage_User_Info::Manage_User_Info(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Manage_User_Info)
{
    ui->setupUi(this);


    QLabel *my_lable = new QLabel(this);
    my_lable->setGeometry(0, 0, this->width(), this->height());
    QPixmap pixmap(":/imgBackground/background.jpg");
    pixmap.scaled(this->size());
    my_lable->setPixmap(pixmap);
    my_lable->lower();

#if 1
    connect(ui->btnQuery, SIGNAL(clicked()), this, SLOT(to_query()));
    connect(ui->btnQuery_All, SIGNAL(clicked()), this, SLOT(to_query_all()));
    connect(ui->btnDelete, SIGNAL(clicked()), this, SLOT(to_delete()));
    connect(ui->btnDelete_All, SIGNAL(clicked()), this, 	SLOT(to_delete_all()));
    connect(ui->btnExit,SIGNAL(clicked()), this, SLOT(close()));
    connect(ui->btnModifyUserInfo, SIGNAL(clicked()), this, SLOT(to_modify_userInfo()));
    connect(ui->btnModifyWeightInfo, SIGNAL(clicked()), this, SLOT(to_modify_weightInfo()));
    connect(ui->btnSelect, SIGNAL(clicked()), this, SLOT(to_select()));

    ui->leQuery->installEventFilter(this);

    data_helper = new dataHelper();

    key = new keyBoard();
    key->setWindowModality(Qt::ApplicationModal);

    my_cbo = new cbo_select(this);
    my_cbo->setGeometry(ui->btnSelect->x(), ui->btnSelect->y(), my_cbo->width(), my_cbo->height());
    my_cbo->hide();
    connect(my_cbo, SIGNAL(re_select(QString,int)), this, SLOT(to_re_select(QString, int)));

    connect(key, SIGNAL(Edit(QString)), this, SLOT(to_edit(QString)));
    connect(key, SIGNAL(Hide()), this, SLOT(to_reshow()));

    ui->btnExit->setFocus();
    _getAllUserInfo();
    this->setAttribute(Qt::WA_DeleteOnClose);
   QGraphicsDropShadowEffect *shadow_effect_1 = new QGraphicsDropShadowEffect(this);
   QGraphicsDropShadowEffect *shadow_effect_2 = new QGraphicsDropShadowEffect(this);
   shadow_effect_1->setOffset(5, 5);
   shadow_effect_1->setColor(Qt::gray);
   shadow_effect_1->setBlurRadius(8);
   shadow_effect_2->setOffset(5, 5);
   shadow_effect_2->setColor(Qt::gray);
   shadow_effect_2->setBlurRadius(8);

   ui->leQuery->setGraphicsEffect(shadow_effect_1);
   ui->tableUserInfo->setGraphicsEffect(shadow_effect_2);


#endif
}

Manage_User_Info::~Manage_User_Info()
{
#if 1
    delete data_helper;
    delete key;
#endif
    delete ui;
}

/*******************slot***********************/


#if 1

void Manage_User_Info::to_select()
{
    this->my_cbo->show();
}

void Manage_User_Info::to_re_select(QString str, int index)
{
    ui->btnSelect->setText(str);
    this->my_cbo->hide();
    ui->leQuery->clear();
    ui->leQuery->setPlaceholderText("请输入" + str);  //无输入时,显示该文本
    ui->btnExit->setFocus();
    cur_index = index;
}


//根据输入框中的内容进行模糊匹配查询
void Manage_User_Info::to_query()
{
    Message_Box *message_box = new Message_Box();
    message_box->setTitle("注意");
    message_box->setImg(Message_Box::Warning);
    message_box->setShowButton(false, true);
    if (ui->leQuery->text().trimmed() == "") {
        message_box->setText("输入信息不能为空！");
        message_box->exec();
        return;
    }
    QString query_info[4] = {
        "Id",
        "Name",
        "Sex",
        "Height"
    };

    QString select = QString("select * from userInfo where " +
                             query_info[cur_index] +
                             " like '%"+ "%1%' ").arg(ui->leQuery->text().trimmed());
    if (!data_helper->Select(select)) {
        message_box->setText("查无相关用户！");
        message_box->exec();
        return;
    }
    member_info.clear();
    data_helper->Select(select, &member_info);
    _clearTableViewData();
    _setTableViewData();
}

//查询所有的用户信息
void Manage_User_Info::to_query_all()
{
    if (member_info.size() == 0) {
        Message_Box *message_box = new Message_Box();
        message_box->setTitle("注意");
        message_box->setText("无任何用户信息！");
        message_box->setImg(Message_Box::Warning);
        message_box->setShowButton(false, true);
        message_box->exec();
        return;
    }
    _getAllUserInfo();
    _clearTableViewData();
    _setTableViewData();
}

void Manage_User_Info::to_delete()
{
    if (!_isSelectAnyUser()) {
        return ;
    }

    Message_Box *message_box = new Message_Box();
    message_box->setTitle("询问");
    message_box->setText("是否删除？");
    message_box->setImg(Message_Box::Question);
    message_box->setShowButton(true, true);

    int table_id = ui->tableUserInfo->currentIndex().row();
    if (message_box->exec() == QDialog::Rejected) {
        return;
    }

    QString Id = member_info.at(table_id).getId();
    QString sql_delete_login = QString("delete from Login where Id = '%1' ").arg(Id);
    QString sql_delete_userInfo = QString("delete from userInfo where Id = '%1' ").arg(Id);
    QString sql_delete_weightInfo = QString("delete from WeightInfo where Id = '%1' ").arg(Id);

    data_helper->Delete(sql_delete_login); //删除用户登录表中的数据
    data_helper->Delete(sql_delete_userInfo); //删除用户信息表中的数据
    data_helper->Delete(sql_delete_weightInfo); //删除用户体重信息表中的数据

    member_info.removeAt(table_id);
    _clearTableViewData();
    _setTableViewData();
}

void Manage_User_Info::to_delete_all()
{
    if (member_info.size() == 0) {
        Message_Box *message_box = new Message_Box();
        message_box->setTitle("注意");
        message_box->setText("无任何用户信息！");
        message_box->setImg(Message_Box::Warning);
        message_box->setShowButton(false, true);
        message_box->exec();
        return;
    }

    Message_Box *message_box = new Message_Box();
    message_box->setTitle("询问");
    message_box->setText("是否删除？");
    message_box->setImg(Message_Box::Question);
    message_box->setShowButton(true, true);
    if (message_box->exec() == QDialog::Rejected) {
        return;
    }

    QString sql_delete_all_login = QString("delete from Login ");
    QString sql_delete_all_userInfo = QString("delete from userInfo ");
    QString sql_delete_all_weightInfo = QString("delete from WeightInfo ");

    data_helper->Delete(sql_delete_all_login); //删除用户登录表中的数据
    data_helper->Delete(sql_delete_all_userInfo); //删除用户信息表中的数据
    data_helper->Delete(sql_delete_all_weightInfo); //删除用户体重信息表中的数据

    member_info.clear();
    _clearTableViewData();
    _setTableViewData();
}

void Manage_User_Info::to_edit(QString str)
{
    ui->leQuery->setText(str);
    key->setLeText(ui->leQuery->text());
}

void Manage_User_Info::to_reshow()
{
    key->hide();
    ui->btnExit->setFocus();
}

//修改用户个人信息
void Manage_User_Info::to_modify_userInfo()
{
        if (!_isSelectAnyUser()) {
            return;
        }
        int table_id = ui->tableUserInfo->currentIndex().row();

        m_u_i = new  modify_user_info();
        m_u_i->setUserInfo(member_info.at(table_id));
        m_u_i->setInterfaceInfo();
        connect(m_u_i, SIGNAL(sucess(Member_Info)), this, SLOT(to_reset_tableview(Member_Info)));
        m_u_i->show();

}

//修改用户体重信息
void Manage_User_Info::to_modify_weightInfo()
{
    if (!_isSelectAnyUser()) {
        return;
    }
    int table_id = ui->tableUserInfo->currentIndex().row();
    detail_weight = new Detail_Weight();
    detail_weight->setAdminMode();
    detail_weight->setUserWeight(member_info.at(table_id).getId());
    detail_weight->initInterface();
    detail_weight->show();
}

//修改信息后,重新更新tableview中单个用户的数据
void Manage_User_Info::to_reset_tableview(Member_Info mem_info)
{
        int table_id = ui->tableUserInfo->currentIndex().row();
        if (member_info.at(table_id) != mem_info) {
            (member_info.operator [](table_id)) = mem_info;
        }

        QAbstractItemModel *model = ui->tableUserInfo->model();
        QStandardItem *item;

        for(int i = 1; i < model->columnCount(); i++) {
            item = ((QStandardItemModel *)model)->item(table_id,i);
            if (i == 1)
                item->setText(mem_info.getPwd());
            else if (i == 2)
                item->setText(mem_info.getName());
            else if (i == 3)
                item->setText(mem_info.getSex());
            else
                item->setText(mem_info.getHeight());
        }

}

/**********************************************/


//设置表格的数据
void Manage_User_Info::_setTableViewData()
{
    QStandardItemModel  *model = new QStandardItemModel();
    //用户名Id,密码(Pwd),姓名,性别,身高
    model->setColumnCount(5);
    model->setHeaderData(0, Qt::Horizontal, "用户名");
    model->setHeaderData(1, Qt::Horizontal, "密码");
    model->setHeaderData(2, Qt::Horizontal, "姓名");
    model->setHeaderData(3, Qt::Horizontal, "性别");
    model->setHeaderData(4, Qt::Horizontal, "身高(cm)");

    for(int i = 0; i < member_info.size(); i++)
    {
        model->setItem(i, 0, new QStandardItem(member_info.at(i).getId()));
        model->setItem(i, 1, new QStandardItem(member_info.at(i).getPwd()));
        model->setItem(i, 2, new QStandardItem(member_info.at(i).getName()));
        model->setItem(i, 3, new QStandardItem(member_info.at(i).getSex()));
        model->setItem(i, 4, new QStandardItem(member_info.at(i).getHeight()));

        model->item(i,0)->setTextAlignment(Qt::AlignCenter);
        model->item(i,1)->setTextAlignment(Qt::AlignCenter);
        model->item(i,2)->setTextAlignment(Qt::AlignCenter);
        model->item(i,3)->setTextAlignment(Qt::AlignCenter);
        model->item(i,4)->setTextAlignment(Qt::AlignCenter);
    }

    ui->tableUserInfo->setModel(model);

}

//清除表中的数据
void Manage_User_Info::_clearTableViewData()
{
    QAbstractItemModel *model  = ui->tableUserInfo->model();
    delete model;
}

//初始化界面
void Manage_User_Info::initInterface()
{
    ui->leQuery->setPlaceholderText("请输入用户名");  //无输入时,显示该文本

    _setTableViewData();

    //设置列宽可伸缩
    ui->tableUserInfo->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);

    //设置窗口禁止最大化和最小化
    this->setWindowFlags(this->windowFlags() &~ Qt::WindowMinMaxButtonsHint);

    //使tabview内的内容无法编辑
    ui->tableUserInfo->setEditTriggers ( QAbstractItemView::NoEditTriggers );

    //选中一整行
    ui->tableUserInfo->setSelectionBehavior(QAbstractItemView::SelectionBehavior::SelectRows);

    //只能选中单行
    ui->tableUserInfo->setSelectionMode(QAbstractItemView::SingleSelection);

    ui->leQuery->setMaxLength(20);

    setWindowModality(Qt::ApplicationModal); //设置模态



    this->setFixedSize(this->width(), this->height());

    ui->btnExit->setFocus();
}

//判断是否选择任何用户
bool Manage_User_Info::_isSelectAnyUser()
{
    int table_id = ui->tableUserInfo->currentIndex().row();
    if (table_id < 0) {
        Message_Box *message_box = new Message_Box();
        message_box->setTitle("注意");
        message_box->setText("未选择任何用户！");
        message_box->setImg(Message_Box::Warning);
        message_box->setShowButton(false, true);
        message_box->exec();
        return false;
    }
    return true;
}

//获得所有用户信息
void Manage_User_Info::_getAllUserInfo()
{
    QString select_all = QString("select * from userInfo");
    member_info.clear();
    data_helper->Select(select_all, &member_info);
}

void Manage_User_Info::mousePressEvent(QMouseEvent *event)
{
    my_cbo->hide();
    QWidget::mousePressEvent(event);
}

//过滤器
bool Manage_User_Info::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == ui->leQuery) {
        if (event->type() == QEvent::FocusIn) {
            key->show();
            key->setLeText(ui->leQuery->text());
            return false;
        }
    } else {
        key->hide();
        ui->btnExit->setFocus();
    }
    return false;
}

#endif
