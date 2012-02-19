#ifndef __DATACTRL_H__
#define __DATACTRL_H__

#include <QtGui/QStandardItemModel>

#include "cell.h"

class DataCtrl : public QStandardItemModel
{
  Q_OBJECT

  public:
    enum EMode
    {
      eModeView,
      eModeEdit,
      eModeDefineCentroid,
    };

  public:
    class CSVDataType
    {
      public:
        CSVDataType(const QString &name, const QString &suffix, QString (*function)(const DataCtrl *, const Cell&)):
          name(name),
          suffix(suffix),
          function(function) {}
        CSVDataType(const CSVDataType &other) :
          name(other.name),
          suffix(other.suffix),
          function(other.function) {}

      protected:
        friend class QVector<CSVDataType>;
        CSVDataType() {}
        const CSVDataType &operator=(const CSVDataType &other)
        {
          name=other.name;
          suffix=other.suffix;
          function=other.function;
          return *this;
        }

      public:
        QString value(const DataCtrl *me, const Cell &cell) const { return (*function)(me, cell); }

      public:
        QString name;
        QString suffix;

      protected:
        QString (*function)(const DataCtrl *, const Cell&);
    };

  public:
    DataCtrl(QObject *parent = NULL);
    virtual ~DataCtrl() {}

    void addPoint(const QPointF &point);

    void draw() const;

    bool isSaved() const { return saved; }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;

  public slots:
    void removeLastPoint();
    void finalizeForm();
    void removeLastForm();

    void clear();

    void save(const QString &filename);
    void load(const QString &filename);
    void exportCsv(const QString &filename);
    QString getCsvSuffix() const;

    void setCurrentMode(const EMode mode) { cntMode = mode; }
    EMode currentMode() const { return cntMode; }

  protected slots:
    void refresh();

  signals:
    void countChanged(int, int);
    void angleChanged(int);
    void rowsInserted(const QModelIndex &parent, int first, int last);

  protected:
    friend class Settings;

    bool saved;
    EMode cntMode;
    CellPolygon points;
    Cell cell;
    QVector<Cell> cells;
    QVector<CellPolygon> centroidsRef;
    static QVector<CSVDataType> csvDataTypes;
    QVector<CSVDataType*> csvSelection;
    static QColor centroidsRefColor;
    qreal averageAngle;
    qreal averageCenroidRadius;

    QStandardItem *rootItem;
};

#endif
