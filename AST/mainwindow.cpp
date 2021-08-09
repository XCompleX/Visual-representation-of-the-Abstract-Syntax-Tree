#include "mainwindow.h"
#include "Parser.h"
#include "Lexer.h"
#include "ExpressionNode.h"
#include "ui_mainwindow.h"
#include <vector>

MainWindow::MainWindow(QWidget *parent): QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_pushButton_clicked() {
    QString code = ui->textEdit->toPlainText();

    Lexer l(code.toStdString());
    std::vector<Token> TokenList = l.lexAnalisys();

    /*for(Token t : TokenList){
        std::cout << "Token{\n";
        std::cout << t.type.name << "\n";
        std::cout << t.type.regex << "\n";
        std::cout << t.pos << "\n";
        std::cout << t.text << "\n";
        std::cout << "}\n";
    }*/

    Parser parser(TokenList);
    parser.parseCode(ui);
}
