#ifndef PARSER_H
#define PARSER_H
#include <mainwindow.h>
#include "ui_mainwindow.h"
#include "Token.h"
#include <QDebug>
#include <vector>
#include "ExpressionNode.h"
#include "StatementsNode.h"
#include "NumberNode.h"
#include "VariableNode.h"
#include "BinOperationNode.h"
#include "UnarOpearotationNode.h"
#include <map>

class Parser{
  std::vector<Token> tokens;
  int pos = 0;
  std::map<std::string, ExpressionNode*> scope;
  std::vector<std::string> Errors;
  public:
      Parser(std::vector<Token> tokens){
          this->tokens = tokens;
      }
      Token match(std::vector<TokenType> type){
          if(this->pos < this->tokens.size()){
              Token currentToken = this->tokens[this->pos];
              for(TokenType i : type){
                  if(i.name == currentToken.type.name){
                      this->pos += 1;
                      return currentToken;
                  }
              }
          }
          TokenType tt("null","null");
          Token null(tt,"NULL",0);
          return null;
      }
      Token require(std::vector<TokenType> type){
          const Token token = this->match(type);
          if(token.type.name == "null"){
              Errors.push_back("Error on position " + std::to_string(this->pos) + " need " + type[0].name);
          }
          return token;
      }

      ExpressionNode* parseVariableOrNumber(){
          std::vector<TokenType> tt;
          tt.push_back(TokenType("NUMBER", "^[0-9]*"));
          Token number = this->match(tt);
          if(number.type.name != "null"){
              NumberNode* a = new NumberNode(number);
              return a;
          }
          tt[0] = TokenType("VARIABLE", "^[a-z]*");
          Token variable = this->match(tt);
          if(variable.type.name != "null"){
              VariableNode* a = new VariableNode(variable);
              return a;
          }
          Errors.push_back("Need variable or number on position " + std::to_string(this->pos));
      }

      ExpressionNode* parsePrint(){
          std::vector<TokenType> tt;
          tt.push_back(TokenType("LOG", "^CONSOLE"));
          Token operatorLog = this->match(tt);
          if(operatorLog.type.name != "null"){
              UnarOperationNode* uon = new UnarOperationNode(operatorLog, this->parseFormula());
              return uon;

          }
          Errors.push_back("Need unar operator CONSOLE on pos " + std::to_string(this->pos));
      }

      ExpressionNode* parseParentTheses(){
          std::vector<TokenType> tt;
          tt.push_back(TokenType("LPAR", "^\\("));
          if(this->match(tt).type.name != "null"){
              ExpressionNode* node = this->parseFormula();
              tt[0] = TokenType("RPAR", "^\\)");
              this->require(tt);
              return node;
          }
          else{
              return this->parseVariableOrNumber();
          }
      }

      ExpressionNode* parseFormula(){
        std::vector<TokenType> tt;
        tt.push_back(TokenType("MINUS", "^\\-"));
        tt.push_back(TokenType("PLUS", "^\\+"));
        ExpressionNode* leftNode= this->parseParentTheses();
        Token operatoR = this->match(tt);
        while(operatoR.type.name != "null"){
            ExpressionNode* rightNode = this->parseParentTheses();
            leftNode = new BinOperationNode(operatoR,leftNode,rightNode);
            operatoR = this->match(tt);
        }
        return leftNode;
      }

      ExpressionNode* parseExpression(){
          std::vector<TokenType> tt;
          tt.push_back(TokenType("VARIABLE", "^[a-z]*"));
          if(this->match(tt).type.name == "null"){
              ExpressionNode* printNode = this->parsePrint();
              return printNode;
          }
          this->pos -= 1;
          ExpressionNode* variableNode = this->parseVariableOrNumber();
          tt[0] = TokenType("ASSIGN", "^\\=");
          Token assignOperator = this->match(tt);
          if(assignOperator.type.name != "null"){
              ExpressionNode* rightFormulaNode = this->parseFormula();
              BinOperationNode* binaryNode = new BinOperationNode(assignOperator, variableNode, rightFormulaNode);
              return binaryNode;
          }
          Errors.push_back("After variable need ASSIGN on pos " + std::to_string(this->pos));
      }
      void parseCode(Ui::MainWindow *ui){
          ui->answer->clear();
          ui->treeWidget->clear();
          ui->treeWidget->update();
          TokenType tt("null","null");
          Token null(tt,"NULL",0);
          StatementsNode* root = new StatementsNode(null);
          root->Statementsnode = "Statementsnode";
          while(this->pos < this->tokens.size()){
              ExpressionNode* codeStringNode = this->parseExpression();
              if(Errors.size() > 0){
                  ui->answer->setFixedHeight(221);
                  ui->answer->setFixedWidth(771);
                  ui->answer->setText(QString::fromStdString(Errors[0]));
                  return;
              }
              std::vector<TokenType> tt;
              tt.push_back(TokenType("SEMICOLON", "^;"));
              this->require(tt);
              root->addNode(codeStringNode);
          }
          ui->answer->setFixedHeight(0);
          ui->answer->setFixedWidth(0);
          for(ExpressionNode* i : root->codeStrings){
              QTreeWidgetItem * item = new QTreeWidgetItem();
              run(i, ui, item);
          }
          ui->treeWidget->expandAll();
      }

      /*void run(ExpressionNode* node, int num, Ui::MainWindow *ui){
          if(node->token.type.name == "NUMBER"){
             ui->answer->setText(ui->answer->toPlainText() + QString::fromStdString(((NumberNode*)node)->token.text));
          }
          if(node->token.type.name == "LOG"){
             ui->answer->setText(ui->answer->toPlainText() + "CONSOLE\n");
             ui->answer->setText(ui->answer->toPlainText() + " -> \n");
             this->run(((UnarOperationNode*)node)->operand, num + 1, ui);
          }
          if(node->token.type.name == "PLUS"){
             ui->answer->setText(ui->answer->toPlainText() + "+\n");
             ui->answer->setText(ui->answer->toPlainText() + QString::fromStdString(std::to_string(num)) + ".1) -> ");
             this->run(((BinOperationNode*)node)->leftNode, num + 1, ui);
             ui->answer->setText(ui->answer->toPlainText()  + "\n" +  QString::fromStdString(std::to_string(num)) + ".2) -> ");
             this->run(((BinOperationNode*)node)->rightNode, num + 1, ui);
             ui->answer->setText(ui->answer->toPlainText() + "\n");
          }
          if(node->token.type.name == "MINUS"){
              ui->answer->setText(ui->answer->toPlainText() + "-\n");
              ui->answer->setText(ui->answer->toPlainText() + QString::fromStdString(std::to_string(num)) + ".1) -> ");
              this->run(((BinOperationNode*)node)->leftNode, num + 1, ui);
              ui->answer->setText(ui->answer->toPlainText()  + "\n" +  QString::fromStdString(std::to_string(num)) + ".2) -> ");
              this->run(((BinOperationNode*)node)->rightNode, num + 1, ui);
              ui->answer->setText(ui->answer->toPlainText() + "\n");
          }
          if(node->token.type.name == "ASSIGN"){
            VariableNode* variable = ((VariableNode*)((BinOperationNode*)node)->leftNode);
            ui->answer->setText(ui->answer->toPlainText() + "=\n");
            ui->answer->setText(ui->answer->toPlainText() + QString::fromStdString(std::to_string(num)) + ".1) -> " + QString::fromStdString(variable->token.text));
            this->scope[variable->token.text] = ((BinOperationNode*)node)->rightNode;
            ui->answer->setText(ui->answer->toPlainText() + "\n" + QString::fromStdString(std::to_string(num)) + ".2) -> ");
            this->run(((BinOperationNode*)node)->rightNode, num + 1, ui);
            ui->answer->setText(ui->answer->toPlainText() + "\n");
          }
          if(node->token.type.name == "VARIABLE"){
              if(this->scope[node->token.text])
                  ui->answer->setText(ui->answer->toPlainText() + QString::fromStdString(node->token.text));
              else
                  ui->answer->setText(ui->answer->toPlainText() + "The variable: " + QString::fromStdString(node->token.text) + " not founded\n");
          }
          if(node->Statementsnode== "StatementsNode"){
              for(ExpressionNode* i: ((StatementsNode*)node)->codeStrings)
                  run(i, num + 1, ui);
          }
      }*/
      void run(ExpressionNode* node, Ui::MainWindow *ui, QTreeWidgetItem* item){
          if(node->token.type.name == "NUMBER"){
             QTreeWidgetItem * number = new QTreeWidgetItem();
             number->setText(0, QString::fromStdString(((NumberNode*)node)->token.text));
             item->addChild(number);
          }
          if(node->token.type.name == "LOG"){
             ui->treeWidget->addTopLevelItem(item);
             item->setText(0,"CONSOLE");
             this->run(((UnarOperationNode*)node)->operand, ui, item);
          }
          if(node->token.type.name == "PLUS"){
             QTreeWidgetItem * plus = new QTreeWidgetItem();
             plus->setText(0, "+");
             item->addChild(plus);
             this->run(((BinOperationNode*)node)->leftNode, ui, plus);
             this->run(((BinOperationNode*)node)->rightNode, ui, plus);
          }
          if(node->token.type.name == "MINUS"){
              QTreeWidgetItem * minus = new QTreeWidgetItem();
              minus->setText(0, "-");
              item->addChild(minus);
              this->run(((BinOperationNode*)node)->leftNode, ui, minus);
              this->run(((BinOperationNode*)node)->rightNode, ui, minus);
          }
          if(node->token.type.name == "ASSIGN"){
            VariableNode* variable = ((VariableNode*)((BinOperationNode*)node)->leftNode);
            ui->treeWidget->addTopLevelItem(item);
            item->setText(0,"=");
            this->scope[variable->token.text] = ((BinOperationNode*)node)->rightNode;
            QTreeWidgetItem * leftNode = new QTreeWidgetItem();
            leftNode->setText(0, QString::fromStdString(variable->token.text));
            item->addChild(leftNode);
            this->run(((BinOperationNode*)node)->rightNode, ui, item);
          }
          if(node->token.type.name == "VARIABLE"){
              if(this->scope[node->token.text]){
                  QTreeWidgetItem * variable = new QTreeWidgetItem();
                  variable->setText(0, QString::fromStdString(node->token.text));
                  item->addChild(variable);
              }
              else{
                  ui->answer->setFixedHeight(221);
                  ui->answer->setFixedWidth(771);
                  ui->treeWidget->clear();
                  ui->treeWidget->update();
                  ui->answer->setText(ui->answer->text() + "The variable: " + QString::fromStdString(node->token.text) + " not founded\n");
              }
          }
          if(node->Statementsnode== "StatementsNode"){
              for(ExpressionNode* i: ((StatementsNode*)node)->codeStrings){
                  QTreeWidgetItem * item = new QTreeWidgetItem();
                  run(i, ui, item);
              }
          }
      }
};

#endif // PARSER_H
