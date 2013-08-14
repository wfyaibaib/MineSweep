#include"block_area.h"
#include<algorithm>
#include<QEvent>
#include<QGridLayout>
#include<QLayout>
#include<QMouseEvent>
#include<QObject>
#include<QQueue>
#include<QTime>
#include <vector>
#include<QWidget>

ptrdiff_t random(ptrdiff_t i) 
{ 
	return qrand()%i;
}

ptrdiff_t (*p_random)(ptrdiff_t)=random;

BlockArea::BlockArea(int row,int column,int mine_number,QWidget* parent)
	:QWidget(parent)
{
	set_block_area(row,column,mine_number,true);
}

void BlockArea::set_block_area(int row,int column,int mine_number,int init_flag)
{
        if(!init_flag){ // 析构原来的
		for(int i=0;i<row_;i++)
			for(int j=0;j<column_;j++)
				delete static_cast<Block*>(mainLayout->itemAtPosition(i,j)->widget());
		delete mainLayout;
	}
	
	row_=row;
	column_=column;
	total_block_number_=row_*column_;
	total_mine_number_=mine_number;
	turn_over_block_number_=0;
	game_over_flag_=false;
        // 数组中保存 是否是雷的信息，两段

    std::vector<bool> mine_flag(total_block_number_);
	for(int i=0;i<total_mine_number_;i++)
		mine_flag[i]=true;
	for(int i=total_mine_number_;i<total_block_number_;i++)
		mine_flag[i]=false;
	QTime time;
	time=QTime::currentTime();
        qsrand(time.msec()+time.second()*1000);// 当前时间毫秒数作种子
    std::random_shuffle(mine_flag.begin(),mine_flag.begin()+total_block_number_,p_random);

        // 网格布局
	mainLayout=new QGridLayout(this);
	for(int i=0;i<row_;i++)
		for(int j=0;j<column_;j++)
			mainLayout->addWidget(new Block(mine_flag[i*column_+j]),i,j);

	for(int i=0;i<row_;i++){
		for(int j=0;j<column_;j++){
			Block* current_block=static_cast<Block*>(mainLayout->itemAtPosition(i,j)->widget());
                        // 设置每个块周围雷数
			current_block->set_number(calculate_mines(i,j));
                        // 每个块被翻开 是不是雷 信号与整个区域 被翻开槽关联
			connect(current_block,SIGNAL(turn_over(bool)),this,SLOT(slot_turn_over(bool)));
                        // 当前块在整个区域安装 事件过滤器，针对每个块安装
			current_block->installEventFilter(this);
		}		
	}
}

bool	BlockArea::eventFilter(QObject* watched, QEvent* event)
{
        if(game_over_flag_) // 如果游戏已经结束， 对区域内鼠标按键消息过滤
		if(event->type()==QEvent::MouseButtonPress)
			return true;
	return QWidget::eventFilter(watched,event);
}

void BlockArea::slot_turn_over(bool is_mine)
{
        if(is_mine){ // 踩到雷，标记游戏结束，发送信号
		game_over_flag_=true;
		emit game_over(false);
        }else{  // 不是雷
                ++turn_over_block_number_; // 已经被翻开的++
                if(turn_over_block_number_==total_block_number_-total_mine_number_){ // 剩下的都是雷
			game_over_flag_=true;
			emit game_over(true);
		}else{
			for(int i=0;i<row_;i++)
				for(int j=0;j<column_;j++)
					if(sender()==static_cast<Block*>(mainLayout->itemAtPosition(i,j)->widget()))
                                                try_to_turn_over_more_blocks(i,j);//取得谁发送翻开信号，找到位置，扩散
		}
	}
}

int BlockArea::calculate_mines(int x,int y)const
{
	int number=0;
	for(int i=0;i<3;i++)
		for(int j=0;j<3;j++)
			if( (x-1+i>=0) && (x-1+i<row_) && (y-1+j>=0) && (y-1+j<column_) )	
				if(static_cast<Block*>(mainLayout->itemAtPosition(x-1+i,y-1+j)->widget())->is_mine())
					++number;
	return number;
}

void BlockArea::try_to_turn_over_more_blocks(int x,int y)
{
	QQueue<QPair<int,int> > queue;
	QPair<int,int> pair;
	queue.enqueue(qMakePair(x,y));
	while(!queue.isEmpty()){
		pair=queue.head();
		queue.dequeue();
		if(calculate_mines(pair.first,pair.second)==0){
			for(int i=0;i<3;i++){
				for(int j=0;j<3;j++){
					if( (pair.first-1+i>=0) && (pair.first-1+i<row_) && (pair.second-1+j>=0) && (pair.second-1+j<column_) ){
						if(!static_cast<Block*>(mainLayout->itemAtPosition(pair.first-1+i,pair.second-1+j)->widget())->is_turn_over()){
							static_cast<Block*>(mainLayout->itemAtPosition(pair.first-1+i,pair.second-1+j)->widget())->turn_over();
							++turn_over_block_number_;
							queue.enqueue(qMakePair(pair.first-1+i,pair.second-1+j));
						}
					}
				}
			}
		}
	}	
	if(turn_over_block_number_==total_block_number_-total_mine_number_){
			game_over_flag_=true;
			emit game_over(true);
	}
}
