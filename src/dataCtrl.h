#ifndef __DATACTRL_H__
#define __DATACTRL_H__

#include <QtCore/QAbstractItemModel>

#include <functional>

#include "cell.h"

using namespace std;

class DataCtrl : public QAbstractItemModel
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
        CSVDataType(const QString &name, const QString &suffix, function<QString (const Cell&)> fun):
          name(name),
          suffix(suffix),
          fun(fun) {}
        CSVDataType(const CSVDataType &other) :
          name(other.name),
          suffix(other.suffix),
          fun(other.fun) {}

      protected:
        friend class QVector<CSVDataType>;
        CSVDataType() {}
        const CSVDataType &operator=(const CSVDataType &other)
        {
          name=other.name;
          suffix=other.suffix;
          fun=other.fun;
          return *this;
        }

      public:
        inline QString value(const Cell &cell) const { return fun(cell); }

      public:
        QString name;
        QString suffix;

      protected:
        function<QString (const Cell&)> fun;
    };

  public:
    DataCtrl(QObject *parent = NULL);
    virtual ~DataCtrl() {}

    void addPoint(const QPointF &point);

    void draw() const;

    bool isSaved() const { return saved; }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    static int maxCSD() { return maximalCSD; }

    static void getDataTypesNames(QStringList &names);
    static void getSelectedDataTypesNames(QStringList &names);
    static void setSelectedDataTypesNames(const QStringList &names);

  public slots:
    void removeLastPoint();
    void finalizeForm();
    void removeLastForm();
    void removeSelectedForm();
    void startEditSelectedForm();
    void stopEditSelectedForm();

    void setMaximalCSD(const int& maxCSD);

    void setSelection(const QModelIndex &selected);

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
    void countChanged(int, int, int);
    void angleVPatchChanged(int);
    void angleVBeatingChanged(int);

  protected:
    friend class Settings;
    friend class SettingsView;

    bool saved;
    EMode cntMode;
    XPolygon points;
    CellItem cell;
    QVector<Cell> cells;
    QVector<XPolygon> centroidsRef;
    static int maximalCSD;
    static QVector<CSVDataType> csvDataTypes;
    static QVector<const CSVDataType*> csvSelection;
    static QColor centroidsRefColor;
    qreal averageAngleVPatch;
    qreal averageAngleVBeating;
    qreal averageCenroidRadius;
};

#endif
