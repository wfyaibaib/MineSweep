#ifndef MAIN_WINDOW_H_
#define MAIN_WINDOW_H_

#include"block_area.h"
#include<QMainWindow>
#include<QTime>
#include<QTimer>
class QAction;
class QActionGroup;
class QCloseEvent;
class QMenu;
class QToolBar;
class QWidget;

class MainWindow:public QMainWindow
{
	Q_OBJECT
public:
    //
	MainWindow(QWidget* parent=0);

protected:
    // 关闭事件
	void closeEvent(QCloseEvent* event);
	
private slots:
        void slot_new_game();   // 新游戏
        void slot_rank();   // 排名
        void slot_show_game_toolBar(bool show); // 显示工具条
        void slot_show_statusBar(bool show);    // 显示状态条
        void slot_standard(QAction* standard_action);   // 难度
        void slot_about_game(); // 关于游戏
        void slot_game_over(bool is_win);   // 游戏结束
        void slot_timer();  // 时间槽

private:
        void read_settings();   // ?
        void write_settings();  // ?
	void create_actions();
	void create_menus();
	void create_game_toolBar();
	void create_statusBar();

private:
        BlockArea* area_;   // 扫雷区
        int row_;       //
	int column_;
	int mine_number_;
	int easy_record_time_;
	int middle_record_time_;
	int hard_record_time_;
	QString easy_record_name_;
	QString middle_record_name_;
	QString hard_record_name_;
        int current_standard_;  // 现有难度
	
	QMenu* game_menu;
	QMenu* setting_menu;
	QMenu* help_menu;

	QToolBar* game_toolBar;

	QAction* new_game_action;
	QAction* rank_action;
	QAction* exit_action;
	QAction* show_game_toolBar_action;
	QAction* show_statusBar_action;
	QAction* easy_standard_action;
	QAction* middle_standard_action;
	QAction* hard_standard_action;
	QAction* custom_standard_action;
	QActionGroup* standard_actionGroup;
	QAction* about_game_action;
	QAction* about_qt_action;
	
	QLabel* time_label;
	QTime time;
	QTimer timer;
};

#endif
