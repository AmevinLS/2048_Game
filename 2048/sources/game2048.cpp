#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <string>
#include <tuple>
#include <algorithm>

using namespace std;

template<typename T>
ostream& operator<<(ostream& os, vector<vector<T>> m)
{
    for(int i=0;i<m.size();i++)
    {
        for(int j=0;j<m[i].size();j++)
        {
            os << m[i][j] << " ";
        }
        os << endl;
    }
    return os;
}

ostream& operator<<(ostream& os, vector<tuple<int,int>> v)
{
    for(int i=0;i<v.size();i++)
    {
        os << "(" << get<0>(v[i]) << "," << get<1>(v[i]) << ") ";
    }
    os << endl;
    return os;
}

ostream& operator<<(ostream& os, vector<tuple<tuple<int,int>,tuple<int,int>>> transes)
{
    for(auto& trans:transes)
    {
        os << "(" << get<0>(get<0>(trans)) << "," << get<1>(get<0>(trans)) << ")->(" << get<0>(get<1>(trans)) << "," << get<1>(get<1>(trans)) << ")";
    }
    os << endl;
    return os;
}

vector<vector<int>> transpose(vector<vector<int>> m)
{
    vector<vector<int>> res = m;
    for(int i=0;i<m.size();i++)
    {
        for(int j=0;j<m[i].size();j++)
        {
            res[j][i] = m[i][j];
        }
    }
    return res;
}

enum Move_type
{
    UNKNOWN,
    UP,
    DOWN,
    LEFT,
    RIGHT
};

enum Game_state
{
    ONGOING,
	WON,
    LOST
};

class Game
{
public:
    Game(int n);
  
    void change_size(int new_n);
    void restart();
    void make_move(Move_type m);
    bool check_finished();
	
	Game_state check_state();


	vector<vector<int>> get_field()
    {
        return field;
    }
	
    int get_n()
    {
        return n_field;
    }
	
    vector<tuple<tuple<int,int>,tuple<int,int>>> get_last_trans()
    {
        return last_trans;
    }

    vector<tuple<int,int>> get_last_double()
    {
        return last_double;
    }
	
	vector<int> get_last_double_num()
	{
		return last_double_num;
	}

	vector<tuple<int,int>> get_start_cells()
	{
		return start_cells;
	}

	tuple<int,int> get_last_added()
	{
		return last_added;
	}
	
	Move_type get_last_move()
	{
		return last_move;
	}
	
	Game_state get_state()
	{
		return state;
	}
	
	void set_target_num(int t_num)
	{
		target_num = t_num;
	}
	
	int get_target_num()
	{
		return target_num;
	}
	
private:
    vector<vector<int>> field;
    vector<tuple<tuple<int,int>,tuple<int,int>>> last_trans;
    vector<tuple<int,int>> last_double;
	vector<int> last_double_num;
	vector<tuple<int,int>> start_cells;
	tuple<int,int> last_added;
    Game_state state;
	int target_num;
    Move_type last_move = UNKNOWN;
    int n_field;


    tuple<int,int> add_random();

    void flip_right();
    void flip_up();
    void flip_down();

    vector<int> trans_to_vec_convert(tuple<tuple<int,int>,tuple<int,int>> trans, Move_type m);

    void convert_last();
};


Game::Game(int n)
{
	field = vector<vector<int>>(n, vector<int>(n,0));
	n_field = n;
	restart();
	state = ONGOING;
	target_num = 2048;
}
void Game::change_size(int new_n)
{
	field = vector<vector<int>>(new_n, vector<int>(new_n, 0));
	n_field = new_n;
	state = ONGOING;
}
void Game::restart()
{
	change_size(n_field);
	start_cells.clear();
	start_cells.push_back(add_random());
	start_cells.push_back(add_random());
	state = ONGOING;
}
void Game::make_move(Move_type m)
{
	if(state!=ONGOING)
		return;

	last_trans.clear();
	last_double.clear();
	last_double_num.clear();
	bool move_made = false;
	switch(m)
	{
	case LEFT:
		break;
	case RIGHT:
		flip_right();
		break;
	case UP:
		flip_up();
		break;
	case DOWN:
		flip_down();
		break;
	}

	for(int i=0;i<n_field;i++)
	{
		int j=0;

		for(int st_ind=0;st_ind<n_field-1;st_ind++)
		{
			if(field[i][st_ind]>0)
			{
				int first_full = find_if(field[i].begin()+st_ind+1, field[i].end(), [](int x){return x>0;}) - field[i].begin();
				if(first_full == n_field)
					break;
				if(field[i][first_full] == field[i][st_ind])
				{
					field[i][st_ind] *= 2;
					field[i][first_full] = 0;
					last_double.push_back(make_tuple(i, st_ind));
					last_double_num.push_back(field[i][st_ind]);
					last_trans.push_back(make_tuple(make_tuple(i, first_full), make_tuple(i, st_ind)));
					move_made = true;
				}
			}
			else
			{
				int first_full = find_if(field[i].begin()+st_ind+1, field[i].end(), [](int x){return x>0;}) - field[i].begin();
				if(first_full == n_field)
					break;
				int second_full = find_if(field[i].begin()+first_full+1, field[i].end(), [](int x){return x>0;}) - field[i].begin();
				if(field[i][first_full] == field[i][second_full] && second_full!=n_field)
				{
					field[i][st_ind] = 2 * field[i][first_full];
					field[i][first_full] = 0;
					field[i][second_full] = 0;
					last_double.push_back(make_tuple(i, st_ind));
					last_double_num.push_back(field[i][st_ind]);
					last_trans.push_back(make_tuple(make_tuple(i, first_full), make_tuple(i, st_ind)));
					last_trans.push_back(make_tuple(make_tuple(i, second_full), make_tuple(i, st_ind)));
					move_made = true;
				}
				else
				{
					field[i][st_ind] = field[i][first_full];
					field[i][first_full] = 0;
					last_trans.push_back(make_tuple(make_tuple(i, first_full), make_tuple(i, st_ind)));
					move_made = true;
				}
			}
		}
	}

	switch(m)
	{
	case LEFT:
		break;
	case RIGHT:
		flip_right();
		break;
	case UP:
		flip_up();
		break;
	case DOWN:
		flip_down();
		break;
	}

	if(move_made)
	{
		last_move = m;
		last_added = add_random();
	}
	state = check_state();
	convert_last();
}

bool Game::check_finished()
{
	for(int i=0;i<n_field;i++)
	{
		for(int j=0;j<n_field;j++)
		{
			if(field[i][j]==0)
				return false;
			if(i!=n_field-1)
				if(field[i+1][j]==field[i][j])
					return false;
			if(j!=n_field-1)
				if(field[i][j+1]==field[i][j])
					return false;
		}
	}
	return true;
}

Game_state Game::check_state()
{
	for(int i=0;i<n_field;i++)
	{
		for(int j=0;j<n_field;j++)
		{
			if(field[i][j]==target_num)
				return WON;
		}
	}
	for(int i=0;i<n_field;i++)
	{
		for(int j=0;j<n_field;j++)
		{
			if(field[i][j]==0)
				return ONGOING;
			if(i!=n_field-1)
				if(field[i+1][j]==field[i][j])
					return ONGOING;
			if(j!=n_field-1)
				if(field[i][j+1]==field[i][j])
					return ONGOING;
		}
	}
	return LOST;
}

tuple<int,int> Game::add_random()
{
	vector<int> empty_x;
	vector<int> empty_y;

	for(int i=0;i<n_field;i++)
		for(int j=0;j<n_field;j++)
			if(field[i][j]==0)
			{
				empty_x.push_back(i);
				empty_y.push_back(j);
			}
	int n_empty = rand()%empty_x.size();
	field[empty_x[n_empty]][empty_y[n_empty]] = 2;
	return make_tuple(empty_x[n_empty], empty_y[n_empty]);
}

void Game::flip_right()
{
	vector<vector<int>> temp_field = field;
	for(int i=0;i<n_field;i++)
	{
		for(int j=0;j<n_field;j++)
		{
			field[i][j] = temp_field[i][n_field-j-1];
		}
	}
}
void Game::flip_up()
{
	vector<vector<int>> temp_field = field;
	for(int i=0;i<n_field;i++)
	{
		for(int j=0;j<n_field;j++)
		{
			field[i][j] = temp_field[j][i];
		}
	}
}
void Game::flip_down()
{
	vector<vector<int>> temp_field = field;
	for(int i=0;i<n_field;i++)
	{
		for(int j=0;j<n_field;j++)
		{
			field[i][j] = temp_field[n_field-j-1][n_field-i-1];
		}
	}
}

vector<int> Game::trans_to_vec_convert(tuple<tuple<int,int>,tuple<int,int>> trans, Move_type m)
{
	vector<int> res;
	res.push_back(get<0>(get<0>(trans)));
	res.push_back(get<1>(get<0>(trans)));
	res.push_back(get<0>(get<1>(trans)));
	res.push_back(get<1>(get<1>(trans)));
	switch(last_move)
	{
	case LEFT:
		break;
	case RIGHT:
		res[1] = n_field-res[1]-1;
		res[3] = n_field-res[3]-1;
		break;
	case UP:
		swap(res[0],res[1]);
		swap(res[2],res[3]);
		break;
	case DOWN:
		swap(res[0],res[1]);
		swap(res[2],res[3]);
		res[0] = n_field-res[0]-1;
		res[1] = n_field-res[1]-1;
		res[2] = n_field-res[2]-1;
		res[3] = n_field-res[3]-1;
		break;
	}
	return res;
}

void Game::convert_last()
{
	int mult_1=1, mult_2=1, add_1=0, add_2=0;
	switch(last_move)
	{
	case LEFT:
		break;
	case RIGHT:
		for(auto& item:last_double)
		{
			item = make_tuple(get<0>(item), n_field-get<1>(item)-1);
		}
		break;
	case UP:
		for(auto& item:last_double)
		{
			item = make_tuple(get<1>(item), get<0>(item));
		}
		break;
	case DOWN:
		for(auto& item:last_double)
		{
			item = make_tuple(n_field-get<1>(item)-1, n_field-get<0>(item)-1);
		}
		break;
	}
	for(int i=0;i<last_trans.size();i++)
	{
		vector<int> v = trans_to_vec_convert(last_trans[i], last_move);
		last_trans[i] = make_tuple(make_tuple(v[0],v[1]), make_tuple(v[2],v[3]));
	}
}