/**
   *******************************************
   * @file      qcptooltip.h
   * @author    yifan
   * @brief     None
   * @attention None
   * @date      2023/9/4
   ********************************************
   */


#ifndef QCPTOOLTIP_GRAPH_H
#define QCPTOOLTIP_GRAPH_H

#include "util/qcustomplot.h"
#include "util/qcptooltip.h"

/**
 * @brief 用于折线图的tooltip
 */
class QCPGraphToolTip : public QCPToolTip {
    Q_OBJECT
public:
    explicit QCPGraphToolTip(QCustomPlot *parentPlot);

protected:

    QCPGraph *m_highlight_graph;

    void drawScatterPoint(QCPPainter *painter, const QPointF &pos) override;

    void updatePosition(const QPointF &newPos, bool replot = false) override;
};

#endif //QCPTOOLTIP_GRAPH_H
