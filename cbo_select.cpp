#include "cbo_select.h"
#include "ui_cbo_select.h"

cbo_select::cbo_select(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::cbo_select)
{
    ui->setupUi(this);


    connect(ui->btn_user, SIGNAL(clicked()), this, SLOT(to_user()));
    connect(ui->btn_name, SIGNAL(clicked()), this, SLOT(to_name()));
    connect(ui->btn_sex, SIGNAL(clicked()), this, SLOT(to_sex()));
    connect(ui->btn_height, SIGNAL(clicked()), this, SLOT(to_height()));
    this->setFixedSize(this->size());
}

cbo_select::~cbo_select()
{
    delete ui;
}

void cbo_select::to_user()
{
    emit re_select(ui->btn_user->text().trimmed(), 0);
}

void cbo_select::to_name()
{
    emit re_select(ui->btn_name->text().trimmed(), 1);
}

void cbo_select::to_sex()
{
    emit re_select(ui->btn_sex->text().trimmed(), 2);
}

void cbo_select::to_height()
{
    emit re_select(ui->btn_height->text().trimmed(), 3);
}
