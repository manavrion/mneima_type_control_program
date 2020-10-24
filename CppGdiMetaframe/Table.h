#pragma once
#include "Panel.h"
namespace MetaFrame {

    class Table : public Panel {

    public:
        Table() : n(), m() {};

        Table *setColomnsNumber(int m) {
            this->m = m;
            for (auto &ob : table) {
                ob.resize(m);
            }
            build();
            return this;
        }

        Table *setStringsNumber(int n) {
            this->n = n;
            table.resize(n);
            for (auto &ob : table) {
                ob.resize(m);
            }
            build();
            return this;
        }

        Table *setCell(String s, int i, int j) {
            table[i][j] = s;
            labeltable[i][j]
                ->SetText(s)
                ->setX(40*i)
                ->SetY(20*j)
                ->SetHorizontalAlignment(HorizontalAlignment::Absolute)
                ->SetVerticalAlignment(VerticalAlignment::Absolute);
            return this;
        }


        void refrash() {
            n = table.size();
            m = table[0].size();
            build();
            for (int i = 0; i < table.size(); i++) {
                for (int j = 0; j < table[i].size(); j++) {
                    setCell(table[i][j], i, j);
                }
            }
        }

    protected:
        int n;
        int m;
        std::vector<std::vector<String>> table;
        std::vector<std::vector<Label*>> labeltable;


        Table *build() {
            childs.clear();
            labeltable.clear();
            labeltable.resize(n);
            for (auto &ob : labeltable) {
                ob.resize(m);
                for (auto &obj : ob) {
                    obj = new Label();
                    Add(obj);
                }
            }
            return this;
        }

        /*virtual void Repaint(Graphics *graphics) {
            graphics->fillRectangle(Rect(0, 0, width, height), this->getBackgroundColor());
            graphics->drawRectangle(Rect(0, 0, width, height), Color(255, 255, 255));
            graphics->drawLine(PointF(3, 20), PointF(40*n, 20), Color(255, 255, 255), 1);
            graphics->drawLine(PointF(40, 3), PointF(40, 20*m), Color(255, 255, 255), 1);
        };*/

    public:
        std::vector<std::vector<String>>& getTable() {
            return table;
        };
        
        ~Table() {
            childs.clear();
            labeltable.clear();
        };
    
    };

}