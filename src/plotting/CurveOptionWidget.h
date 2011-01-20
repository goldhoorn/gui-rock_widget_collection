#ifndef CURVEOPTIONWIDGET_H
#define	CURVEOPTIONWIDGET_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QComboBox>
#include <QSpinBox>
#include <QFrame>
#include <QScrollArea>
#include <QLineEdit>

#include <qwt_plot_curve.h>

#include "ColorPickerButton.h"

/**
 * Widget which displays all curves which are currently on the plot.
 * One can configure some attributes of the curves, namely:
 * <ul>
 * <li>Color</li>
 * <li>Width</li>
 * <li>Line Style</li>
 * </ul>
 */
class CurveOptionWidget : public QWidget
{

    Q_OBJECT
    
public:
    /**
     * Constructor
     */
    CurveOptionWidget();

    /**
     * Destructor
     */
    virtual ~CurveOptionWidget();

    /**
     * Initializes the widget with the given curves
     * displaying their properties
     * @param curves the curves to display
     */
    void initializeLayout(std::vector<QwtPlotCurve*> curves);

    /**
     * Updates the existing curves with the values changed
     * on the widget
     */
    void updateExistingCurves();

protected:
    /** Layout of trhe widget*/
    QGridLayout layout;
    /** All Buttons to pick colors from*/
    std::vector<ColorPickerButton*> buttons;
    /** All Boxes to select line style from*/
    std::vector<QComboBox*> boxes;
    /** All curves which can be changed*/
    std::vector<QwtPlotCurve*> curves;
    /** SpinBoxes to change the width*/
    std::vector<QSpinBox*> spinBoxes;
    /** Scroll Area if there are mor curves than are displayable*/
    QScrollArea scrollArea;
    /** Edit Fields to change the name*/
    std::vector<QLineEdit*> nameEdits;

private:

};

#endif	/* CURVEOPTIONWIDGET_H */
