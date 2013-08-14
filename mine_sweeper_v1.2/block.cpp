#include"block.h"
#include<QLabel>
#include<QMouseEvent>
#include<QPixmap>
#include<QWidget>

Block::Block(bool mine_flag,QWidget* parent)
	:QLabel(parent)
{
        mine_flag_=mine_flag;   //
	mark_flag_=false;
	turn_over_flag_=false;
	number_=-1;
	setPixmap(QPixmap(":/images/normal.png"));
}

void Block::set_number(int number)
{
	number_=number;
}

void Block::turn_over()
{
        if(!turn_over_flag_){           // 没有被翻开，就翻开
		turn_over_flag_=true;
                if(mine_flag_)              // 是雷，载入图片，否则根据周围雷数载入相应图片
			setPixmap(QPixmap(":/images/mine.png"));
		else
			setPixmap(QPixmap(":/images/mine_"+QString("%1").arg(number_)+".png"));
                update();                   // 更新?
	}
}

bool Block::is_mine()const
{
	return mine_flag_;
}

bool Block::is_turn_over()const
{
	return turn_over_flag_;
}

void Block::mousePressEvent(QMouseEvent* event)
{
	if(event->button()==Qt::LeftButton){
                if(!turn_over_flag_&&!mark_flag_){ // 没有被翻开，没有旗帜
			turn_over_flag_=true;
                        if(mine_flag_==true){           // 是雷， 发 被 翻转 是雷 信号
				setPixmap(QPixmap(":/images/mine.png"));
				update();
				emit turn_over(true);
                        }else{                              // 不是雷， 发 被 翻转  不是雷 信号
				setPixmap(QPixmap(":/images/mine_"+QString("%1").arg(number_)+".png"));
				update();
				emit turn_over(false);
			}
		}
	}else if(event->button()==Qt::RightButton){
                if(!turn_over_flag_){  //
			if(!mark_flag_){
				mark_flag_=true;
				setPixmap(QPixmap(":/images/flag.png"));
			}else{
				mark_flag_=false;
				setPixmap(QPixmap(":/images/normal.png"));
			}
			update();
		}	
	}
        QLabel::mousePressEvent(event); // 调用基类事件处理
}
