#include"main_window.h"
#include<QAction>
#include<QActionGroup>
#include<QApplication>
#include<QDialog>
#include<QDialogButtonBox>
#include<QHBoxLayout>
#include<QIcon>
#include<QInputDialog>
#include<QLayout>
#include<QMainWindow>
#include<QMenu>
#include<QMenuBar>
#include<QMessageBox>
#include<QPushButton>
#include<QSettings>
#include<QSpinBox>
#include<QStatusBar>
#include<QToolBar>
#include<QVBoxLayout>
#include<QWidget>
#include <QTextCodec>
const QString g_software_name="MineSweep";  //窗口标题
const QString g_software_version="1.0";
const QString g_software_author=("unknown");

const int g_no_record_time=10000;   //游戏纪录
const QString g_no_record_name="";

MainWindow::MainWindow(QWidget* parent)
	:QMainWindow(parent)
{
        area_=new BlockArea(9,9,10); // 行列 ，雷数
        connect(area_,SIGNAL(game_over(bool)),this,SLOT(slot_game_over(bool)));//游戏结束
        setCentralWidget(area_);//中心部件
	
	create_actions();
	create_menus();
	create_game_toolBar();
	create_statusBar();

	QCoreApplication::setOrganizationName(g_software_author);
	QCoreApplication::setApplicationName(g_software_name);
	
	read_settings();
	
	layout()->setSizeConstraint(QLayout::SetFixedSize);

	setWindowTitle(g_software_name);
	setWindowIcon(QIcon(":/game.png"));
	
	timer.start(1000);
	connect(&timer,SIGNAL(timeout()),this,SLOT(slot_timer()));
	slot_new_game();
}

void MainWindow::closeEvent(QCloseEvent*)
{
	write_settings();
}

void MainWindow::slot_new_game()
{
	area_->set_block_area(row_,column_,mine_number_);
	time_label->setText("0");
	time.restart();
	timer.start();
}

void MainWindow::slot_rank()
{
	QDialog dialog;
	dialog.setWindowTitle(tr("rank"));
	
	QGridLayout* up_layout=new QGridLayout;
	up_layout->addWidget(new QLabel(tr("Standard")),0,0);
	up_layout->addWidget(new QLabel(tr("Time")),0,1);
	up_layout->addWidget(new QLabel(tr("Name")),0,2);
	up_layout->addWidget(new QLabel(tr("Easy")),1,0);
	up_layout->addWidget(new QLabel(QString("%1").arg(easy_record_time_)),1,1);
	up_layout->addWidget(new QLabel(easy_record_name_),1,2);
	up_layout->addWidget(new QLabel(tr("Middle")),2,0);
	up_layout->addWidget(new QLabel(QString("%1").arg(middle_record_time_)),2,1);
	up_layout->addWidget(new QLabel(middle_record_name_),2,2);
	up_layout->addWidget(new QLabel(tr("Hard")),3,0);
	up_layout->addWidget(new QLabel(QString("%1").arg(hard_record_time_)),3,1);
	up_layout->addWidget(new QLabel(hard_record_name_),3,2);
	
	QPushButton* recount_button=new QPushButton(tr("recount"));
	QPushButton* close_button=new QPushButton(tr("close"));
	close_button->setDefault(true);
	connect(recount_button,SIGNAL(clicked()),&dialog,SLOT(accept()));
	connect(close_button,SIGNAL(clicked()),&dialog,SLOT(reject()));
	
	QHBoxLayout* bottom_layout=new QHBoxLayout;
	bottom_layout->addStretch();
	bottom_layout->addWidget(recount_button);
	bottom_layout->addWidget(close_button);
		
	QVBoxLayout* main_layout=new QVBoxLayout(&dialog);
	main_layout->addLayout(up_layout);
	main_layout->addLayout(bottom_layout);
	
	if(dialog.exec()==QDialog::Accepted){
			easy_record_time_=middle_record_time_=hard_record_time_=g_no_record_time;
			easy_record_name_=middle_record_name_=hard_record_name_=g_no_record_name;
	}
}

void MainWindow::slot_show_game_toolBar(bool show)
{
	if(show)
		game_toolBar->show();
	else
		game_toolBar->hide();
}

void MainWindow::slot_show_statusBar(bool show)
{
	if(show)
		statusBar()->show();
	else
		statusBar()->hide();
}
	
void MainWindow::slot_standard(QAction* standard_action)
{
	if(standard_action==easy_standard_action){
		current_standard_=0;
		row_=9;
		column_=9;
		mine_number_=10;
	}else if(standard_action==middle_standard_action){
		current_standard_=1;
		row_=16;
		column_=16;
		mine_number_=40;
	}else if(standard_action==hard_standard_action){
		current_standard_=2;
		row_=16;
		column_=30;
		mine_number_=99;
	}else if(standard_action==custom_standard_action){
		QDialog dialog;
		dialog.setWindowTitle(tr("set standard"));
		
		QSpinBox* row_spinBox=new QSpinBox;
		row_spinBox->setRange(5,50);
		row_spinBox->setValue(row_);
		QSpinBox* column_spinBox=new QSpinBox;
		column_spinBox->setRange(5,50);
		column_spinBox->setValue(column_);
		QSpinBox* mine_spinBox=new QSpinBox;
		mine_spinBox->setValue(mine_number_);
		
		QHBoxLayout* up_layout=new QHBoxLayout;
		up_layout->addWidget(row_spinBox);
		up_layout->addWidget(column_spinBox);
		up_layout->addWidget(mine_spinBox);
		
		QDialogButtonBox* dialog_buttonBox=new QDialogButtonBox;
		dialog_buttonBox->addButton(QDialogButtonBox::Ok);
		dialog_buttonBox->addButton(QDialogButtonBox::Cancel);
		connect(dialog_buttonBox,SIGNAL(accepted()),&dialog,SLOT(accept()));
		connect(dialog_buttonBox,SIGNAL(rejected()),&dialog,SLOT(reject()));
		
		QHBoxLayout* bottom_layout=new QHBoxLayout;
		bottom_layout->addStretch();
		bottom_layout->addWidget(dialog_buttonBox);
		
		QVBoxLayout* main_layout=new QVBoxLayout(&dialog);
		main_layout->addLayout(up_layout);
		main_layout->addLayout(bottom_layout);
		if(dialog.exec()==QDialog::Accepted)
			if(row_spinBox->value()*column_spinBox->value()>mine_spinBox->value()){
				current_standard_=3;
				row_=row_spinBox->value();
				column_=column_spinBox->value();
				mine_number_=mine_spinBox->value();
			}
	}
	slot_new_game();
}

void MainWindow::slot_about_game()
{
	QString introduction(
		"<h2>"+tr("About Mine Sweepr")+"</h2>"
		+"<p>"+tr("This game is played by revealing squares of the grid, typically by clicking them with a mouse. If a square containing a mine is revealed, the player loses the game. Otherwise, a digit is revealed in the square, indicating the number of adjacent squares (out of the possible eight) that contain mines.if this number is zero then the square appears blank, and the surrounding squares are automatically also revealed. By using logic, the player can in many instances use this information to deduce that certain other squares are mine-free, in which case they may be safely revealed, or mine-filled, in which they can be marked as such (which is effected by right-clicking the square and indicated by a flag graphic).")+"</p>"

		+"<p>"+tr("This program is free software; you can redistribute it and/or modify.it under the terms of the GNU General Public License as published by the Software Foundation; either version 3 of the License, or (at your option) any later version.")+"</p>"
		+"<p>"+tr("Please see ")+"<a href=http://www.gnu.org/licenses/gpl-3.0.html>http://www.gnu.org/licenses/gpl-3.0.html</a>"+tr(" for an overview of GPLv3 licensing")+"</p>"
		+"<br>"+tr("Version:")+g_software_version+"</br>"
		+"<br>"+tr("Author: ")+g_software_author+"</br>"
	);
    // 1 信息框, 2 标题, 3 显示文本, 4 按钮
	QMessageBox messageBox(QMessageBox::Information,tr("About Mine Sweeper"),introduction,QMessageBox::Ok);
    messageBox.exec(); // 运行
}

void MainWindow::slot_game_over(bool is_win)
{
	timer.stop();
	QString name;
	if(is_win){
		switch(current_standard_){
			case 0:
				if(time_label->text().toInt()<easy_record_time_){
					name=QInputDialog::getText(this,tr("Please enter your name"),tr("You create a record. Please enter your name"));
					if(!name.isEmpty()){
						easy_record_time_=time_label->text().toInt();
						easy_record_name_=name;
					}
				}else
					QMessageBox::information(this,tr("Result"),tr("You win"));
				break;
			case 1:
				if(time_label->text().toInt()<middle_record_time_){
					name=QInputDialog::getText(this,tr("Please enter your name"),tr("You create a record. Please enter your name"));
					if(!name.isEmpty()){
						middle_record_time_=time_label->text().toInt();
						middle_record_name_=name;
					}
				}else
					QMessageBox::information(this,tr("Result"),tr("You win"));
				break;
			case 2:
				if(time_label->text().toInt()<hard_record_time_){
					name=QInputDialog::getText(this,tr("Please enter your name"),tr("You create a record. Please enter your name"));
					if(!name.isEmpty()){
						hard_record_time_=time_label->text().toInt();
						hard_record_name_=name;
					}
				}else
					QMessageBox::information(this,tr("Result"),tr("You win"));
				break;
			default:QMessageBox::information(this,tr("Result"),tr("You win"));
		}
	}else{
		QMessageBox::information(this,tr("Result"),tr("You lose"));
	}
}

void MainWindow::slot_timer()//刷新标签时间
{
	time_label->setText(QString("%1").arg(time.elapsed()/1000));
}
	
void MainWindow::read_settings()
{
	QSettings settings;
	settings.beginGroup("MainWindow");
	resize(settings.value("size").toSize());
	move(settings.value("pos").toPoint());
	bool show_game_toolBar=settings.value("showGameToolBar").toBool();
	show_game_toolBar_action->setChecked(show_game_toolBar);
	slot_show_game_toolBar(show_game_toolBar);
	bool show_statusBar=settings.value("showStatusBar").toBool();
	show_statusBar_action->setChecked(show_statusBar);
	slot_show_statusBar(show_statusBar);	 
	settings.endGroup();
	
	settings.beginGroup("GameSetting");
	current_standard_=settings.value("current_standard").toInt();
	switch(current_standard_){
		case 0:easy_standard_action->setChecked(true);break;
		case 1:middle_standard_action->setChecked(true);break;
		case 2:hard_standard_action->setChecked(true);break;
		case 3:custom_standard_action->setChecked(true);break;
		default:;
	}
	row_=settings.value("row").toInt()==0?9:settings.value("row").toInt();
	column_=settings.value("column").toInt()==0?9:settings.value("column").toInt();
	mine_number_=settings.value("mine_number").toInt()==0?10:settings.value("mine_number").toInt();
	settings.endGroup();
	
	settings.beginGroup("Rank");
	easy_record_time_=settings.value("easy_record_time").toInt()==0?g_no_record_time:settings.value("easy_record_time").toInt();
	middle_record_time_=settings.value("middle_record_time").toInt()==0?g_no_record_time:settings.value("middle_record_time").toInt();
	hard_record_time_=settings.value("hard_record_time").toInt()==0?g_no_record_time:settings.value("hard_record_time").toInt();
	easy_record_name_=settings.value("easy_record_name").toString()==""?g_no_record_name:settings.value("easy_record_name").toString();
	middle_record_name_=settings.value("middle_record_name").toString()==""?g_no_record_name:settings.value("middle_record_name").toString();
	hard_record_name_=settings.value("hard_record_name").toString()==""?g_no_record_name:settings.value("hard_record_name").toString();
	settings.endGroup();
}

void MainWindow::write_settings()
{
	QSettings settings;
	
	settings.beginGroup("MainWindow");
	settings.setValue("size",size());
	settings.setValue("pos", pos());
	settings.setValue("showGameToolBar",show_game_toolBar_action->isChecked());
	settings.setValue("showStatusBar",show_statusBar_action->isChecked());
	settings.endGroup();
	
	settings.beginGroup("GameSetting");
	settings.setValue("current_standard",current_standard_);
	settings.setValue("row",row_);
	settings.setValue("column",column_);
	settings.setValue("mine_number",mine_number_);
	settings.endGroup();
	
	settings.beginGroup("Rank");
	settings.setValue("easy_record_time",easy_record_time_);
	settings.setValue("middle_record_time",middle_record_time_);
	settings.setValue("hard_record_time",hard_record_time_);
	settings.setValue("easy_record_name",easy_record_name_);
	settings.setValue("middle_record_name",middle_record_name_);
	settings.setValue("hard_record_name",hard_record_name_);
	settings.endGroup();
	
}
	
void MainWindow::create_actions()
{
            // 新建
	new_game_action=new QAction(QIcon(":/images/new_game.png"),tr("New Game"),this);		
	new_game_action->setShortcut(QKeySequence::New);
	connect(new_game_action,SIGNAL(triggered()),this,SLOT(slot_new_game()));
            // 排名
	rank_action=new QAction(QIcon(":/images/rank.png"),tr("Rank"),this);
	connect(rank_action,SIGNAL(triggered()),this,SLOT(slot_rank()));
            // 退出
	exit_action=new QAction(QIcon(":/images/exit.png"),tr("Exit"),this);
	exit_action->setShortcut(QKeySequence::Quit);
	connect(exit_action,SIGNAL(triggered()),this,SLOT(close()));
            // 是否显示工具条
	show_game_toolBar_action=new QAction(tr("Show Game Tool Bar"),this);
	show_game_toolBar_action->setCheckable(true);
	connect(show_game_toolBar_action,SIGNAL(toggled(bool)),this,SLOT(slot_show_game_toolBar(bool)));
            // 是否显示状态栏
	show_statusBar_action=new QAction(tr("Show Status Bar"),this);
	show_statusBar_action->setCheckable(true);
	connect(show_statusBar_action,SIGNAL(toggled(bool)),this,SLOT(slot_show_statusBar(bool)));
            // 选择难度
	easy_standard_action=new QAction(QIcon(":/images/easy_standard.png"),tr("Easy"),this);
	easy_standard_action->setCheckable(true);
	middle_standard_action=new QAction(QIcon(":/images/middle_standard.png"),tr("Middle"),this);
	middle_standard_action->setCheckable(true);
	hard_standard_action=new QAction(QIcon(":/images/hard_standard.png"),tr("Hard"),this);
	hard_standard_action->setCheckable(true);
	custom_standard_action=new QAction(QIcon(":/images/custom_standard.png"),tr("Custom"),this);
	custom_standard_action->setCheckable(true);
            // 难度组合
	standard_actionGroup=new QActionGroup(this);
	standard_actionGroup->addAction(easy_standard_action);
	standard_actionGroup->addAction(middle_standard_action);
	standard_actionGroup->addAction(hard_standard_action);
	standard_actionGroup->addAction(custom_standard_action);
	connect(standard_actionGroup,SIGNAL(triggered(QAction*)),this,SLOT(slot_standard(QAction*)));
            // 关于游戏
	about_game_action=new QAction(QIcon(":/images/game.png"),tr("About Game"),this);	
	connect(about_game_action,SIGNAL(triggered()),this,SLOT(slot_about_game()));
            // 关于qt
	about_qt_action=new QAction(QIcon(":/images/qt.png"),tr("About Qt"),this);
	connect(about_qt_action,SIGNAL(triggered()),qApp,SLOT(aboutQt()));
}

void MainWindow::create_menus()
{
    // 游戏菜单
	game_menu=menuBar()->addMenu(tr("Game"));
	game_menu->addAction(new_game_action);
	game_menu->addSeparator();	
	game_menu->addAction(rank_action);
	game_menu->addSeparator();
	game_menu->addAction(exit_action);
    // 设置菜单
	setting_menu=menuBar()->addMenu(tr("Setting"));
	setting_menu->addAction(show_game_toolBar_action);
	setting_menu->addAction(show_statusBar_action);
	setting_menu->addSeparator();
	setting_menu->addAction(easy_standard_action);
	setting_menu->addAction(middle_standard_action);
	setting_menu->addAction(hard_standard_action);
	setting_menu->addAction(custom_standard_action);
    // 帮助菜单
	help_menu=menuBar()->addMenu(tr("Help"));
	help_menu->addAction(about_game_action);
        help_menu->addAction(about_qt_action);
}

void MainWindow::create_game_toolBar()
{
    // 工具条
	game_toolBar=addToolBar(tr("Game Tool Bar"));
	game_toolBar->setFloatable(false);
	game_toolBar->setMovable(false);
	game_toolBar->addAction(new_game_action);
	game_toolBar->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
}

void MainWindow::create_statusBar()
{
    //状态栏
	time_label=new QLabel;
	statusBar()->addPermanentWidget(time_label);
	statusBar()->addPermanentWidget(new QLabel(tr("second")));
}
