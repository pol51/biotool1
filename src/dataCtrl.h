#ifndef __DATACTRL_H__
#define __DATACTRL_H__

#include <QtCore/QAbstractItemModel>

#include <functional>

#include "cell.h"

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
        CSVDataType(const QString &name, const QString &suffix, std::function<QString (const Cell&)> fun):
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
        std::function<QString (const Cell&)> fun;
    };

  public:
    DataCtrl(QObject *parent = NULL);
    ~DataCtrl() {}

    void addPoint(const QPointF &point);

    void draw() const;

    bool isSaved() const { return saved; }

    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex &child) const override;
    int rowCount(const QModelIndex &parent) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

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

    void setImageRealHeigth(const qreal &heigth) { _imageRealHeigth = heigth; }
    void setImageRealWidth(const qreal &width) { _imageRealWidth = width; }
    const qreal &imageRealHeigth() const { return _imageRealHeigth; }
    const qreal &imageRealWidth() const { return _imageRealWidth; }

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

    bool saved = true;
    EMode cntMode = eModeView;
    XPolygon points;
    CellItem cell;
    QVector<Cell> cells;
    QVector<XPolygon> centroidsRef;
    static int maximalCSD;
    static QVector<CSVDataType> csvDataTypes;
    static QVector<const CSVDataType*> csvSelection;
    static QColor centroidsRefColor;
    qreal averageAngleVPatch = 0.f;
    qreal averageAngleVBeating = 0.f;
    qreal averageCenroidRadius = 0.f;
    qreal _imageRealHeigth = 1.f;
    qreal _imageRealWidth = 1.f;
};

#endif
