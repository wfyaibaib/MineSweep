#ifndef BLOCK_AREA_H_
#define BLOCK_AREA_H_

#include"block.h"
#include<QWidget>
class QEvent;
class QGridLayout;
class QObject;

class BlockArea:public QWidget
{
	Q_OBJECT
public:
    // 构造行数，列数，雷数
	BlockArea(int row,int column,int mine_number,QWidget* parent=0);
    // 设置 行，列，雷，是否已经初始化
	void set_block_area(int row,int column,int mine_number,int init_flag=false);

signals:
        void game_over(bool is_win); //  信号， 游戏结束

protected:
        bool	eventFilter(QObject* watched, QEvent* event);   // 过滤
	
private slots:
        void slot_turn_over(bool is_mine);  // 槽 被翻开
	
private:
        int calculate_mines(int x,int y)const;		//计算xy为中心，九宫格雷数
        void try_to_turn_over_more_blocks(int x,int y); // 扩散算法

private:
        QGridLayout* mainLayout;     // 布局
	
	int row_;
	int column_;
	int total_block_number_;
	int total_mine_number_;
        int turn_over_block_number_;    // 已经被翻开块数
	
	bool game_over_flag_;
};

#endif
