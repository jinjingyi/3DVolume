#include "MeasureResultTable.h"

MeasureTable::MeasureTable():
    table(new QTableWidget),
    pointPair(),
    length(),
    vPointPair(),
    vLength(),
    rowCount()
{
}

MeasureTable::~MeasureTable() = default;

void MeasureTable::resetTable(){
    length = 0;
    rowCount = 0;
    while(vPointPair.size()){vPointPair.pop_back();}
    while(vLength.size()){vLength.pop_back();}
    rowCount = 0;
    table->setRowCount(rowCount);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels(QStringList{"表型", "点1", "点2", "长度"});
    table->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
}

void MeasureTable::addItem(std::array<std::array<double, 3>, 2> pointPair)
{
    addPointPair(pointPair);
    rowCount += 1;
    table->setRowCount(rowCount);
    table->setItem(rowCount - 1, 0, new QTableWidgetItem("P1P2"));
    table->setItem(rowCount - 1, 1, new QTableWidgetItem("P1"));
    table->setItem(rowCount - 1, 2, new QTableWidgetItem("P2"));
    QTableWidgetItem *item = new QTableWidgetItem(QString::fromStdString(std::to_string((int)length)));
    item->setBackground(QBrush(QColor(Qt::lightGray)));
    item->setFlags(item->flags() & (~Qt::ItemIsEditable));
    table->setItem(rowCount - 1, 3, item);
}

void MeasureTable::deleteItem()
{
    if (rowCount != 0){
        deletePointPair();
        table->removeRow(rowCount - 1);
        rowCount -= 1;
    }
}

void MeasureTable::addPointPair(std::array<std::array<double, 3>, 2> pointPair)
{
    this->pointPair = pointPair;
    length = getPointPointDistance(pointPair[0], pointPair[1]);
    vPointPair.push_back(pointPair);
    vLength.push_back(length);
}

void MeasureTable::deletePointPair()
{
    vPointPair.pop_back();
    vLength.pop_back();
}


std::array<std::array<double, 3>, 2> MeasureTable::getPointPair()
{
    return pointPair;
}

double MeasureTable::getLength()
{
    return length;
}

std::vector<std::array<std::array<double, 3>, 2>> MeasureTable::getVPointPair()
{
    return vPointPair;
}

std::vector<double> MeasureTable::getVLength()
{
    return vLength;
}

QTableWidget* MeasureTable::getTable()
{
    return table;
}

void MeasureTable::setTable(QTableWidget *table){
    this->table = table;
}

