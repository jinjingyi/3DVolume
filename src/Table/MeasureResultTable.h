#ifndef MEASURERESULTTABLE_H
#define MEASURERESULTTABLE_H

#include <QTableWidget>
#include <QHeaderView>

#include <QStringList>
#include <iostream>

#include "Util/Geometry.h"

class MeasureTable {
public:
    MeasureTable();
    ~MeasureTable();
    
    void resetTable();
    
    void addItem(std::array<std::array<double, 3>, 2> pointPair);
    
    void deleteItem();
    
    void addPointPair(std::array<std::array<double, 3>, 2> pointPair);
    
    void deletePointPair();
    
    std::array<std::array<double, 3>, 2> getPointPair();
    
    double getLength();
    
    std::vector<std::array<std::array<double, 3>, 2>> getVPointPair();
    
    std::vector<double> getVLength();
    
    QTableWidget* getTable();
    
    void setTable(QTableWidget *table);
    
private:
    QTableWidget *table;
    std::array<std::array<double, 3>, 2> pointPair;
    double length;
    std::vector<std::array<std::array<double, 3>, 2>> vPointPair;
    std::vector<double> vLength;
    int rowCount;
};

#endif
