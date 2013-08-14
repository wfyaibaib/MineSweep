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
    // ��������������������
	BlockArea(int row,int column,int mine_number,QWidget* parent=0);
    // ���� �У��У��ף��Ƿ��Ѿ���ʼ��
	void set_block_area(int row,int column,int mine_number,int init_flag=false);

signals:
        void game_over(bool is_win); //  �źţ� ��Ϸ����

protected:
        bool	eventFilter(QObject* watched, QEvent* event);   // ����
	
private slots:
        void slot_turn_over(bool is_mine);  // �� ������
	
private:
        int calculate_mines(int x,int y)const;		//����xyΪ���ģ��Ź�������
        void try_to_turn_over_more_blocks(int x,int y); // ��ɢ�㷨

private:
        QGridLayout* mainLayout;     // ����
	
	int row_;
	int column_;
	int total_block_number_;
	int total_mine_number_;
        int turn_over_block_number_;    // �Ѿ�����������
	
	bool game_over_flag_;
};

#endif
