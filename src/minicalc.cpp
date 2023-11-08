/* Title: MiniCalc Source Code
 * File: src/minicalc.cpp
 * Author: Okulus Dev
 * License: GNU GPL v3
 * -------------------------------------------------------------------------- *
 *   Description: Минималистичный калькулятор научных выражений
 */
#include <iostream>
#include <string>
#include <stack>
#include <cctype>
#include <cmath>
#include <vector>
#include <algorithm>

using namespace std;

void drawGraph(const vector<double>&x, const vector<double>&y) {
	double minX = *min_element(x.begin(), x.end());
	double maxX = *max_element(x.begin(), x.end());
	double minY = *min_element(y.begin(), y.end());
	double maxY = *max_element(y.begin(), y.end());

	int width = 70;
	int height = 35;

	vector<double> scaledX(x.size());
	vector<double> scaledY(y.size());
	
	for (int i=0; i < x.size(); i++) {
		scaledX[i] = (x[i] - minX) / (maxX - minX) * width;
		scaledY[i] = (y[i] - minY) / (maxY - minY) * height;
	}

	vector<vector<char>> graph(height + 1, vector<char>(width + 1, ' '));

	for (int i=0; i < width + 1; i++) {
		graph[height / 2][i] = '-';
	}
	for (int i=0; i < height + 1; i++) {
		graph[i][width / 2] = '|';
	}

	for (int i=0; i < scaledX.size(); i++) {
		int xCoord = round(scaledX[i]) + width / 2;
		int yCoord = height / 2 - round(scaledY[i]);
		
		if (xCoord >= 0 && xCoord <= width && yCoord >= 0 && yCoord <= height) {
			graph[yCoord][xCoord] = '#';
		}
	}

	for (int i=0; i < graph.size(); i++) {
		for (int j=0; j < graph[i].size(); j++) {
			cout << graph[i][j];
		}
		cout << endl;
	}
}

int priority(char op) {
	/* Определяем приоритет выполнения.
	 * 
	 * Входные данные:
	 *  char operator - символ оператора (+, -, *, /)
	 * Выходные данные:
	 *  int 1 - если + или -
	 *  int 2 - елси * или /
	 *  int 0 - если не то, и не другое */
	if (op == '+' || op == '-') {
		return 1;
	} else if (op == '*' || op == '/' || op == 'P') {
		return 2;
	}

	return 0;
}

double applyOperation(double first_num, double second_num, char op) {
	/* Приминяем операцию 
	 *
	 * Входные данные:Н
	 *  double first_num - первое число
	 *  double second_num - второе число
	 *  char operator - оператор
	 *
	 * Выходные данные:
	 *  double <результат операции> */
	switch (op) {
		case '+':
			return first_num + second_num;
		case '-':
			return first_num - second_num;
		case '*':
			return first_num * second_num;
		case '/':
			return first_num / second_num;
		case 'P':
			return first_num * M_PI;
		default:
			return 0;
	}
}

double evaluateExpression(const string&expression) {
	stack<double> numbers;
	stack<char> operators;

	for (int i=0; i < expression.length(); i++) {
		if (isdigit(expression[i]) || expression[i] == '.') {
			string numString;
			while (i < expression.length() && (isdigit(expression[i]) || expression[i] == '.')) {
				numString += expression[i];
				i++;
			}
			i--;
			double num = stod(numString);
			numbers.push(num);
		} else if (expression[i] == '(') {
			operators.push(expression[i]);
		} else if (expression[i] == ')') {
			while (!operators.empty() && operators.top() != '(') {
				double b = numbers.top();
				numbers.pop();
				double a = numbers.top();
				numbers.pop();
				char op = operators.top();
				operators.pop();
				double result = applyOperation(a, b, op);
				numbers.push(result);
			}
			operators.pop();
		} else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/' || expression[i] == 'P') {
			while (!operators.empty() && priority(operators.top()) >= priority(expression[i])) {
				double b = numbers.top();
				numbers.pop();

				double a = numbers.top();
				numbers.pop();
				
				char op = operators.top();
				operators.pop();
				double result = applyOperation(a, b, op);
				numbers.push(result);
			}
			operators.push(expression[i]);
		} else {
			if (expression[i] != ' ') {
				cout << "Ошибка: Символ " << expression[i] << " не является числом, оператором или скобкой" << endl;
				return 0;
			}
		}
	}

	while (!operators.empty()) {
		double b = numbers.top();
		numbers.pop();
		double a = numbers.top();
		numbers.pop();
		char op = operators.top();
		operators.pop();
		
		double result = applyOperation(a, b, op);
		numbers.push(result);
	}

	return numbers.top();
}

int main() {
	string expression;
	bool graphmode = false;

	cout << "MiniCalc v.0.1.1\nВведите q чтобы выйти; Введите help чтобы вывести справку" << endl;
	cout << "Введите выражение" << endl;

	while (true) {
		cout << " >>> ";
		getline(cin, expression);

		if (expression == "q" || expression == "quit") {
			cout << "Вы вышли из MiniCalc" << endl;
			return 0;
		} else if (expression == "help") {
			cout << "q || quit - выход" << endl;
			cout << "graphmode - войти в режим рисования графика" << endl;
		} else if (expression == "graphmode") {
			vector<double> x;
			vector<double> y;

			double first, second; 
				
			cout << "Введите первое число: ";
			cin >> first;

			cout << "Введите второе число: ";
			cin >> second;

			for (double i=first; i <= second; i+= 0.01) {
				x.push_back(i);
				y.push_back(sin(i));
			}

			drawGraph(x, y);
		} else {
			double result = evaluateExpression(expression);
			cout << expression << " = " << result << endl;
		}
	}

	return 0;
}

