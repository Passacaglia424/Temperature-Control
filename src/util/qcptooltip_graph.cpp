/**
   *******************************************
   * @file      qcptooltip.cpp
   * @author    yifan
   * @brief     None
   * @attention None
   * @date      2023/9/4
   ********************************************
   */

#include "util/qcptooltip_graph.h"

QCPGraphToolTip::QCPGraphToolTip(QCustomPlot *parentPlot) :
        QCPToolTip(parentPlot),
        m_highlight_graph(nullptr) {

}


void QCPGraphToolTip::updatePosition(const QPointF &newPos, bool replot) {
    m_highlight_graph = nullptr;
    double tolerance = mParentPlot->selectionTolerance();

    for (int i = mParentPlot->graphCount() - 1; i >= 0; --i) {
        QCPGraph *graph = mParentPlot->graph(i);
        if (!graph->realVisibility() || graph->scatterStyle().isNone()) continue; // graph 不显示
        double limitDistance = tolerance; // limitDistance用于选择的范围
        double penWidth = graph->pen().widthF();
        QCPScatterStyle scatterStyle = graph->scatterStyle();

        limitDistance = qMax(scatterStyle.size(), tolerance);
        penWidth = scatterStyle.isPenDefined() ? scatterStyle.pen().widthF() : penWidth;

        QVariant details;
        double currentDistance = graph->selectTest(newPos, false, &details);
        QCPDataSelection selection = details.value<QCPDataSelection>();
        if (currentDistance >= 0 && currentDistance < limitDistance + penWidth && !selection.isEmpty()) {
            double key = graph->dataMainKey(selection.dataRange().begin());
            double value = graph->dataMainValue(selection.dataRange().begin());
            QPointF pos = graph->coordsToPixels(key, value);
            QRectF rect(pos.x() - limitDistance * 0.5, pos.y() - limitDistance * 0.5, limitDistance, limitDistance);
            rect = rect.adjusted(-penWidth, -penWidth, penWidth, penWidth);
            if (rect.contains(newPos)) {// 鼠标位置是否在数据点上
                setText(QString("x: %1\ny: %2").arg(QString::number(key), QString::number(value)));
                m_highlight_graph = graph;
                m_data_point= pos;
                mParentPlot->setCursor(Qt::PointingHandCursor);
                position->setPixelPosition(newPos);
                setVisible(true);

                if (replot) update();
                break;
            }
        }
    }

    if (!m_highlight_graph && visible()) {
        mParentPlot->setCursor(Qt::ArrowCursor);
        setVisible(false);
        if (replot) update();
    }

}

void QCPGraphToolTip::drawScatterPoint(QCPPainter *painter,const QPointF &pos) {
    auto graph=m_highlight_graph;
    if (!graph) return;
    QCPScatterStyle style = graph->scatterStyle();
    if (style.isNone()) return;

    if (graph->selectionDecorator()) {
        style = graph->selectionDecorator()->getFinalScatterStyle(style);
    }
    style.applyTo(painter, graph->pen());
    style.setSize(style.size() * 1.2);
    style.drawShape(painter, pos);
}





