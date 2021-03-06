// animal_try.cpp: 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <list>



using namespace std;

const int fact_num = 31;      //知识库中的知识:31种知识
const int rule_num = 15;      //知识库中的规则：15条规则
const int rule_volume = 4;    //规则中每个结果最多有4个前提条件
const int object_range_begin = 25;  //从第25个知识开始
const int object_range_end = 31;    //到第31个知识为目标结论
const int object_middle_begin = 21;     //中间结果起始位置

string fact[fact_num] =
{
	"有毛发","产奶","有羽毛","会飞","会下蛋",
	"吃肉","有犬齿","有爪","眼盯前方","有蹄",
	"反刍","黄褐色","有斑点","有黑色条纹","长脖",
	"长腿","不会飞","会游泳","黑白二色","善飞",
	"哺乳类","鸟类","食肉类","蹄类","金钱豹",
	"虎","长颈鹿","斑马","鸵鸟","企鹅","信天翁"
};

int rule_prerequisite[rule_num][rule_volume] =
{
	{ 1,0,0,0 },
{ 2,0,0,0 },
{ 3,0,0,0 },
{ 4,5,0,0 },
{ 21,6,0,0 },
{ 7,8,9,0 },
{ 21,10,0,0 },
{ 21,11,0,0 },
{ 23,12,13,0 },
{ 23,12,14,0 },
{ 24,15,16,13 },
{ 24,14,0,0 },
{ 22,15,16,4 },
{ 22,18,19,4 },
{ 22,20,0,0 }
};

int rule_result[rule_num] =
{
	21,
	21,
	22,
	22,
	23,
	23,
	24,
	24,
	25,
	26,
	27,
	28,
	29,
	30,
	31
};

bool backward_reasoning(int num, int message[]);

bool inference(int num, int message[])         //迭代推理机
{
	int ii, ij, ik, im, in;
	int hit_num = 0;          //输入前提也规则前提重合数
	int prerequisite_num;     //规则前提数
	int *message_c;           //迭代前提
	int num_c;                //迭代前提数量
	for (ik = 0; ik < num; ik++)     //剪枝函数
	{
		if (message[ik] >= object_range_begin && message[ik] <= object_range_end)
		{
			cout << "归并信息：" << fact[message[ik] - 1] << endl;
			cout << "推理成功！" << endl << endl;
			system("pause");
			exit(0);
		}
	}

	for (ii = 0; ii < rule_num; ii++)   //遍历规则匹配
	{
		prerequisite_num = 0;
		hit_num = 0;
		for (ij = 0; ij < rule_volume; ij++)   //计算规则集前提数
		{
			if (rule_prerequisite[ii][ij] == 0)
			{
				break;
			}
			prerequisite_num++;
		}

		for (ij = 0; ij < prerequisite_num; ij++)
		{
			for (ik = 0; ik < num; ik++)
			{
				if (rule_prerequisite[ii][ij] == message[ik])
				{
					hit_num++;
				}
			}
		}

		if (hit_num == prerequisite_num)  //满足某个规则集全部前提
		{
			bool flag;
			for (ik = 0; ik < num; ik++)
			{
				if (message[ik] == rule_result[ii])
				{
					break;
				}
			}
			if (ik == num)
			{
				num_c = num - hit_num + 1;
				flag = true;
			}
			else
			{
				num_c = num - hit_num;
				flag = false;
			}
			message_c = new int[num_c];
			in = 0;
			for (ik = 0; ik < num; ik++)
			{
				for (im = 0; im < hit_num; im++)
				{
					if (rule_prerequisite[ii][im] == message[ik])
					{
						break;
					}
				}
				if (im < hit_num)
				{
					continue;
				}
				message_c[in++] = message[ik];
			}
			if (flag == true)
			{
				message_c[in] = rule_result[ii];
			}
			cout << "推导信息：";
			for (int iz = 0; iz < num; iz++)
			{
				cout << fact[message[iz] - 1] << " ";
			}
			cout << endl;
			return inference(num_c, message_c);
		}
	}
	cout << "归并信息：";
	for (int iz = 0; iz < num; iz++)
	{
		cout << fact[message[iz] - 1] << " ";
	}
	cout << endl;
	backward_reasoning(num, message);
	return false;
}

bool backward_reasoning(int num, int message[])   //反向推理
{
	int ii, ij, ik;
	int prerequisite_num = 0;
	int hit_num = 0;
	int need_rule_number[rule_num];
	int hit_rule_number[rule_num];
	float hit_rule_rate[rule_num];
	float best_hit_rule_rate = 0;
	int best_hit_rule_number;
	int *new_message;
	for (ii = 0; ii < rule_num; ii++)   //遍历规则匹配
	{
		prerequisite_num = 0;
		hit_num = 0;
		for (ij = 0; ij < rule_volume; ij++)   //计算规则集前提数
		{
			if (rule_prerequisite[ii][ij] == 0)
			{
				break;
			}
			prerequisite_num++;
		}

		need_rule_number[ii] = prerequisite_num;
		for (ij = 0; ij < prerequisite_num; ij++)   //计算输入信息命中规则集中的前提数
		{
			for (ik = 0; ik < num; ik++)
			{
				if (rule_prerequisite[ii][ij] == message[ik])
				{
					hit_num++;
				}
			}
		}

		//*******************和正向推理是一样的*****************//
		hit_rule_number[ii] = hit_num;
		hit_rule_rate[ii] = (float)hit_num / prerequisite_num;  //命中率
		for (ij = 0; ij<num; ij++)
		{
			if (message[ij] == rule_result[hit_rule_number[ii]])
			{
				break;
			}
		}
		if (hit_rule_rate[ii] == 1 && ij == num)
		{
			new_message = new int[num + 1];
			for (ik = 0; ik<num; ik++)
			{
				new_message[ik] = message[ik];
			}
			new_message[num] = rule_result[hit_rule_number[ii]];
			num++;
			return inference(num, new_message);
		}
		cout << "rule " << setw(2) << ii << " -> " << setw(8) << fact[rule_result[ii] - 1]
			<< "命中率：" << hit_rule_rate[ii] << endl;
	}
	best_hit_rule_number = -1;
	for (ii = 0; ii<rule_num; ii++)
	{
		if (best_hit_rule_rate<hit_rule_rate[ii] &&
			rule_result[ii] >= object_middle_begin)
		{
			best_hit_rule_rate = hit_rule_rate[ii];
			best_hit_rule_number = ii;
		}
	}
	if (best_hit_rule_number == -1)
	{
		cout << "您输入的信息对本系统无效！按任意键退出..." << endl << endl;
		system("pause");
		exit(0);
	}
	cout << endl;
	cout << "best_hit_rule_number=" << best_hit_rule_number << endl;
	cout << "best_hit_rule_rate=" << best_hit_rule_rate << endl;
	cout << "最佳匹配最终结果=" << fact[rule_result[best_hit_rule_number] - 1] << endl;

	for (ii = 0; ii<need_rule_number[best_hit_rule_number]; ii++)
	{
		for (ij = 0; ij<num; ij++)
		{
			if (rule_prerequisite[best_hit_rule_number][ii] == message[ij])
			{
				break;
			}
		}
		if (ij != num)
		{
			continue;
		}
		else
		{
			if (rule_prerequisite[best_hit_rule_number][ii]<object_middle_begin)
			{
				cout << endl << "请问您持有的信息是否包含\"";
				cout << fact[rule_prerequisite[best_hit_rule_number][ii] - 1];
				cout << "\"?(y or n)" << endl;
				char input;
				while (true)
				{
					cin >> input;
					if (input == 'n')
					{
						new_message = new int[num];
						for (ik = 0; ik<num; ik++)
						{
							new_message[ik] = message[ik];
						}
						break;
					}
					else if (input == 'y')
					{
						new_message = new int[num + 1];
						for (ik = 0; ik<num; ik++)
						{
							new_message[ik] = message[ik];
						}
						new_message[num] = rule_prerequisite[best_hit_rule_number][ii];
						num++;
						return inference(num, new_message);
					}
					else
					{
						cout << "请重新输入（y or n）！";
					}
				}
			}
			else      //询问是否有中间结果rule_prerequisite[best_hit_rule_number][ii]
			{
				int middle_result = rule_prerequisite[best_hit_rule_number][ii];
				for (ii = 0; ii<rule_num; ii++)
				{
					if (rule_result[ii] == middle_result)
					{
						for (ik = 0; ik<need_rule_number[ii]; ik++)
						{
							if (rule_prerequisite[ii][ik] >= object_middle_begin - 1)
							{
								continue;
							}
							for (ij = 0; ij<num; ij++)
							{
								if (rule_prerequisite[ii][ik] == message[ij])
								{
									break;
								}
							}
							if (ij != num)
							{
								continue;
							}
							else
							{
								cout << endl << "请问您持有的信息是否包含\"";
								cout << fact[rule_prerequisite[ii][ik] - 1];
								cout << "\"?(y or n)" << endl;
								char input;
								while (true)
								{
									cin >> input;
									if (input == 'n')
									{
										break;
									}
									else if (input == 'y')
									{
										new_message = new int[num + 1];
										for (int iq = 0; iq<num; iq++)
										{
											new_message[iq] = message[iq];
										}
										new_message[num] = rule_prerequisite[best_hit_rule_number][ii];
										num++;
										return inference(num, new_message);
									}
									else
									{
										cout << "请重新输入（y or n）！";
									}
								}
							}
						}
					}
				}
			}
		}
	}
}

int main(int argc, char **argv)
{
	bool flag;
	int num;
	int *message;
	int ii, ij;
	cout << "《知识库》" << endl;
	for (ii = 0; ii<fact_num; ii++)
	{
		cout << setiosflags(ios::left);
		cout << setw(2) << ii + 1 << ":" << setw(10) << fact[ii] << "  ";
		if (ii % 4 == 0)
		{
			cout << endl;
		}
	}
	cout << endl << endl << "请输入初始信息个数：(数字)" << endl;
	cin >> num;
	message = new int[num];
	cout << "请输入已有信息：(不重复的数字,以空格隔开)" << endl;
	for (ii = 0; ii < num; ii++)
	{
		cin >> message[ii];
	}
	cout << endl << "初始信息：";
	for (ij = 0; ij < num; ij++)
	{
		cout << fact[message[ij] - 1] << " ";
	}
	cout << endl << endl;
	if (!inference(num, message))
	{
		cout << "通过您的输入无法得出结果！" << endl;
	}
	system("pause");
	return 0;
}
