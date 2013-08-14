#ifndef BLOCK_H_
#define BLOCK_H_

#include<QLabel>
class QWidget;

class Block:public QLabel
{
	Q_OBJECT

public:
    // 构造 是否有雷
	explicit Block(bool mine_flag,QWidget* parent=0);
    // 设置周围雷数
	void set_number(int number);
    // 翻转
	void turn_over();
    // 是否是雷
	bool is_mine()const;
    // 是否已经被翻开
	bool is_turn_over()const;
	
signals:
    // 被翻开信号
	void turn_over(bool is_mine);
	
protected:
    // 被点击
	void mousePressEvent(QMouseEvent* event);
	
private:
    // 是否是雷
	bool mine_flag_;
    // 是否有标记
	bool mark_flag_;
    // 是否被翻开
	bool turn_over_flag_;
    // 周围雷数
	int number_;
};

#endif
