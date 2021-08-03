#include <QWidget>
#include <QPushButton>
#include <QDebug>
#include <QLineEdit>
#include <QIntValidator>
#include <QPainter>
#include <QLabel>
#include "cisla.h"
#include <QRadioButton>
#include "checker.h"
#include <QLine>
class Window: public QWidget{
public:
    Window(QWidget* parent=0): QWidget(parent){
        //vytvoreni okna a urceni jeho hranic
        this->setGeometry(510,90,900,900);
        this->setFixedSize(900,900);

        //vytvoreni tlacitka "START" a jeho stylovani
        QPushButton* star=new QPushButton(this);
        star->setText("START");
        star->setStyleSheet("background-color:mintcream; font: bold;");
        star->setGeometry(725,225,150,50);

        //Vytvoreni QLabel
        QLabel* text=new QLabel(this);
        text->setGeometry(225,750,478,100);
        text->setAlignment(Qt::AlignCenter);
        text->setText("Press 'START' to start the game!");
        text->setStyleSheet("font: bold; border: 2px solid whitesmoke; background-color: whitesmoke;");

        //Vytvoreni "sachovnice" pomoci QLineEdit
        QLineEdit* ln;
        QList<QLineEdit*> seznam; // Seznam QLineEditu
        for(int i=0;i<=8;i++){
            for(int j=0;j<=8;j++){
                ln=new QLineEdit(this);
                ln->setGeometry(0,0,50,50);
                ln->move(225+53*i,225+53*j);

                //stylovani QLineEdit a zmena jeho nastaveni tak, aby umoznovalo psat uzivateli pouze cislice (1->9)
                ln->setStyleSheet("font: bold; font-size: 18px;");
                ln->setMaxLength(1);
                ln->setAlignment(Qt::AlignCenter);               // !!!!!!!!!!!!!!!! zmena polohy
                ln->setValidator(new QIntValidator(1,9,this));// cisla (1->9)
                ln->setValidator(new QRegularExpressionValidator(QRegularExpression("[1-9]\\d{0,2}"),this));
                seznam.push_back(ln);
            }
        }

        //vytvoreni RadioButtonu a jejich stylovani
        QRadioButton* radbut;
        QList<QRadioButton*>tlacitka;
        for(int i=0;i<3;i++){
            radbut=new QRadioButton(this);
            radbut->setText(QString("%1").arg(i+1));
            radbut->move(725+50*i,300);
            tlacitka.push_back(radbut);
        }


        //vytvoreni tlacitka "CHECK"=zkontrolovat a jeho stylovani
        QPushButton* check;
        check=new QPushButton(this);
        check->setGeometry(725,650,150,50);
        check->setText("CHECK");
        check->setStyleSheet("background-color:mintcream; font: bold;");
        check->setEnabled(false);

        //stav informuje, jake tlacitko je soucasne aktivni
        int* stav=new int;
        *stav=1;
        QObject::connect(tlacitka.at(0),&QRadioButton::clicked,[stav,check](){
            *stav=1;

            check->setEnabled(false);//*
        });
        QObject::connect(tlacitka.at(1),&QRadioButton::clicked,[stav,check](){
            *stav=2;
            check->setEnabled(false);
        });
        QObject::connect(tlacitka.at(2),&QRadioButton::clicked,[stav,check](){
            *stav=3;
            check->setEnabled(false);
        });

        //predem nastvim, aby RadioBUtton"1" bylo aktivovano
        tlacitka.at(0)->click();

        //Co se stane, az tlacitko "START" zmacknu
        QObject::connect(star,&QPushButton::clicked,[seznam,stav,check,text](){

            text->clear();

            //umoznuje zmacknuti tlaxitka "CHECK"
            check->setEnabled(true);

            //smazat vsechny bunky s cisly a pripravit je k novemu nacteni
            for(int i=0;i<=8;i++){
                for(int j=0;j<=8;j++){
                        seznam.at(9*i+j)->clear();
                        seznam.at(9*i+j)->setStyleSheet("background-color: white; font: bold; font-size: 18px;");
                        seznam.at(9*i+j)->setEnabled(true);
                }
            }

            //podle cisla si zvoli spravny soubor a z nej nacte data
            string file;
            vector<vector<int>>ZADANI;
            if(*stav==1){
                file="sudoku_1";
            }
            else if(*stav==2){
                file="sudoku_2";
            }
            else{
                file="sudoku_3";
            }

            ZADANI=parse_for_vector(file);// cisla.h
            for(int i=0;i<=8;i++){
                for(int j=0;j<=8;j++){
                    if(ZADANI.at(j).at(i)!=0){                                             //neumozni zmenu textu v bunkach s nenulovou hodnotou
                        seznam.at(9*i+j)->setText(QString("%1").arg(ZADANI.at(j).at(i)));
                        seznam.at(9*i+j)->setStyleSheet("background-color: gainsboro; font: bold; font-size: 18px;");
                        seznam.at(9*i+j)->setEnabled(false);
                    }
                }
            }
        });


        QObject::connect(check,&QPushButton::clicked,[seznam,stav,text](){
            //vytvoreni vektoru "kontrola" a zmena jeho velikosti
            std::vector<std::vector<int>>kontrola;
            kontrola.resize(9);

                for(int i=0;i<9;i++){
                    for(int j=0;j<9;j++){
                        if(seznam.at(9*i+j)->text().isEmpty()){                                           //pokud bunka je prazdna, zmeni jeji barvu
                            seznam.at(9*i+j)->setStyleSheet("background-color: red; font: bold; font-size: 18px;");
                        }
                        else{
                            kontrola.at(i).push_back(seznam.at(9*j+i)->text().toInt());                        //v opacnem pripade vlozi cislo do vektoru
                            if(seznam.at(9*i+j)->isEnabled()==true){
                                seznam.at(9*i+j)->setStyleSheet("background-color: white; font: bold; font-size: 18px;");
                            }
                        }
                    }
                }
                if(correct_num(kontrola)==true){ //kontroluje, jestli mame 81 cisel

                    string file;
                    std::vector< std::vector<int>>vzor;// vektor reseni "sudoku"
                    if(*stav==1){
                        file="sudoku_1";
                        vzor=parse_for_vector(file);
                    }
                    else if(*stav==2){
                        file="sudoku_2";
                        vzor=parse_for_vector(file);
                    }
                    else{
                        file="sudoku_3";
                        vzor=parse_for_vector(file);
                    }

                    solve(vzor);        // reseni         "checker.h"

                    int mistake=0;      // pocet chyb
                    for(int i=0;i<9;i++){
                        for(int j=0;j<9;j++){
                            if((seznam.at(9*i+j)->isEnabled()==true)&&(seznam.at(9*i+j)->text().toInt()==vzor.at(j).at(i))){ //spravne cislo v bunce
                                seznam.at(9*i+j)->setStyleSheet("border: 2px solid green; font: bold; font-size: 18px;");
                            }
                            else if((seznam.at(9*i+j)->isEnabled()==true)&&(seznam.at(9*i+j)->text().toInt()!=vzor.at(j).at(i))){ // nespravne cislo v bunce
                                seznam.at(9*i+j)->setStyleSheet("border: 2px solid red; font: bold; font-size: 18px;");
                                mistake++;
                            }
                        }
                    }
                    if(mistake==0){                                      //zadna chyba
                        text->setText("WINNER WINNER CHICKEN DINNER!!!");
                    }
                    else {                                               // vypise pocet chyb
                        text->setText(QString("You have %1 mistakes").arg(mistake));
                    }


                }
                else{ // pokud nejake bunky jsou prazdne, vypise uzivatli, at tam napise cisla
//                    qDebug()<<"fill the empty gaps!!!";
                    text->setText("Fill the empty gaps!!!");
                }
        });
    }
    void paintEvent(QPaintEvent* event){ //vykresli primky
        QPainter* painter=new QPainter(this);
        painter->setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::SquareCap));
        for(int i=0;i<4;i++){
            painter->drawLine(223+159*i,227,223+159*i,696);
            painter->drawLine(223,223+159*i,700,223+159*i);
        }
        painter->end();
    }

};
