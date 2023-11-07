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

using namespace std;

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
	} else if (op == '*' || op == '/') {
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
		} else if (expression[i] == '+' || expression[i] == '-' || expression[i] == '*' || expression[i] == '/') {
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

	cout << "MiniCalc v.0.1.0\nВведите q чтобы выйти" << endl;
	cout << "Введите выражение" << endl;

	while (true) {
		cout << " >>> ";
		getline(cin, expression);

		if (expression == "q" || expression == "quit") {
			cout << "Вы вышли из MiniCalc" << endl;
			return 0;
		}

		double result = evaluateExpression(expression);

		cout << expression << " = " << result << endl;
	}

	return 0;
}

