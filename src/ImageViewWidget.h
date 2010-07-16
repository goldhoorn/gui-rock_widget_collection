/* 
 * File:   ImageViewWidget.h
 * Author: blueck
 *
 * Created on 17. Juni 2010, 14:14
 */

#include "QtGui/qwidget.h"
#include <QtGui/QPaintEvent>
#include <QtGui/QPainter>
#include <QtGui/QImage>
#include <QtGui/QPen>
#include <QtCore/QByteArray>
#include <iostream>
#include <QtCore/QList>
#include "TextItem.h"
#include "LineItem.h"
#include "EllipseItem.h"
#include "PolygonItem.h"
#include "PolylineItem.h"

#ifndef IMAGEVIEWWIDGET_H
#define	IMAGEVIEWWIDGET_H

/**
 * Widget which displays an image. The dimensions and the format needs
 * to be given and images can then be added an will be shown immidiately.
 * <h1>Imaging</h1>
 * <p>
 * If an image which is larger than the specified dimension is added, only
 * the images visible parts (within the dimension specified) will be shown.<br>
 * Alternatively one can add an image and scale it. No scaling will be
 * done if the image already has the proportions given at start. Scaling is
 * costy though. If more images of a different size shall be added the best way
 * is to change the format via changeFormat. This will change the
 * format according to the new dimensions. This should be the default behaviour
 * e.g. if a cameras resolution was changed so is the widgets. Scaling
 * should only be done if a few images are changed or if the images are all quite
 * large and should be scaled to enhance the viewing. Note though, again, that
 * scaling <b>does</b> cost performance.
 * <p>
 * <h1>Drawing shapes<h1>
 * One can draw shapes on top of the image. Currently these are:
 * <ul>
 * <li>Texts</li>
 * <li>Lines</li>
 * <li>Ellipses</li>
 * <li>Rectangles</li>
 * <li>Polylines</li>
 * <li>Polygones</li>
 * </ul>
 * One can add the shapes via addItem or a specific convenience method like
 * addText, addLine etc. Added Items will be displayed on top of the image currently displayed and all
 * successive images. RemoveItems, removes the given item. Alternatively all specific shapes 
 * can be removed or all Items.<br>
 * Items can have a group number and all items with a specific group number can be shown or hidden.
 * Note that hidden is <b>NOT</b> the same as removed. Hidden just does not draw them, removed
 * items can not be retrieved by the widget (even though they might be from outside the class).<br>
 * <b>Note:</b>
 * After adding any shape no repaint will be done. You need to either explicitly call
 * repaint on the widget or add a different image will will automatically have the shapes added
 * ontop of it. Otherwise, if adding multiple shapes, repaint would be called very often which
 * especially when working with video would be a performance waste.
 * 
 * @author Bjoern Lueck
 * @version 0.1
 */
class ImageViewWidget : public QWidget
{

    Q_OBJECT

public:
    /**
     * Initializing the widget with the given format and dimensions
     * For format parameters @seeQImage
     * @param width the width of images put onto the widget
     * @param height the height of images put onto the widget
     * @param format the format from QImage
     */
    ImageViewWidget(int width, int height, QImage::Format format);

    /**
     * Destructor cleaning up
     */
    virtual ~ImageViewWidget();

    /**
     * Changes the format and dimensions of the widget so images with
     * other dimensions or formats can be added
     * @param width the new width
     * @param height the new height
     * @param format the new image format
     */
    void changeFormat(int width, int height, QImage::Format format);

    /**
     * Adds an image to the widget
     * @param data the char data of the image
     * @param numBytes the number of bytes the image data consists of
     */
    void addImage(uchar* data, int numBytes);

    /**
     * Adds an image and scales it. If the given dimensions match the
     * dimensions given to the widget no scaling will be done and addImage(uchar*, int)
     * will be called.
     * @param data the image data
     * @param numBytes the number of bytes of the iamge
     * @param origWidth the width of the image NOT the widget
     * @param origHeight the height of the image NOT the widget
     */
    void addImageAndScale(uchar* data, int numBytes, int origWidth, int origHeight);

    /**
     * Adds a Text to the image and all successive images
     * @param xPos the starting x position
     * @param yPos the starting y position
     * @param groupNr the group number
     * @param color the color of the text
     * @param text the text to be displayed
     * @return pointer to a #seeDrawItem to remove the text afterwards.
     */
    DrawItem* addText(int xPos, int yPos, int groupNr, QColor* color, char* text);

    /**
     * Adds a line to the images and all successive images
     * @param xPos the starting x position
     * @param yPos the starting y position
     * @param groupNr the group number
     * @param color the color of the line
     * @param endX the ending x position
     * @param endY the ending y position
     * @return pointer to a #seeDrawItem to remove the line afterwards
     */
    DrawItem* addLine(int xPos, int yPos, int groupNr, QColor* color, int endX, int endY);

    /**
     * Adds an ellipse to the image and all successive images. the elipse will be drawn
     * within an outlying rectangle specified.<br>
     * The ellipse will not be filles and the border will have the minimum width.
     * @param xPos the top left x position of the rectangle
     * @param yPos the top left y position of the rectangle
     * @param groupNr teh group number
     * @param color the color of the ellipses border
     * @param width the width of the rectangle
     * @param height the height of the rectangle
     * @return pointer to a #seeDrawItem to remove the ellipse afterwards
     */
    DrawItem* addEllipse(int xPos, int yPos, int groupNr, QColor* color, int width, int height);

    /**
     * Adds a rectangle to the image and all successive images.
     * The rectangle will not be filles and the border will have the minimum width.
     * @param xPos the top left x position of the rectangle
     * @param yPos the top left y position of the rectangle
     * @param groupNr the group number
     * @param color the color of the rectangles border
     * @param width the width of the rectangle
     * @param height the height of the rectangle
     * @return pointer to a #seeDrawItem to remove the ellipse afterwards
     */
    DrawItem* addRectangle(int xPos, int yPos, int groupNr, QColor* color, int width, int height);

    /**
     * Adds multiple lines
     * @param groupNr the number of the group
     * @param color the color of the lines
     * @param points the points weher a line starts/ends
     * @param numberOfPoints the number of points in the points array
     * @return pointer to a DrawItem
     */
    DrawItem* addPolyline(int groupNr, QColor* color, QPoint* points, int numberOfPoints);

    /**
     * Adds a Polygon with the given points. An additional line will be drawn between
     * the last and the first poin t given. Polygons can be filles unlike Polylines
     * @param groupNr the group number
     * @param color the color of the lines
     * @param points the points of the polygon
     * @param numberOfPoints the number of points in the points array
     * @return a DrawItem containing the Polygon
     */
    DrawItem* addPolygon(int groupNr, QColor* color, QPoint* points, int numberOfPoints);

    /**
     * Adds a shape to the image
     * @param drawItem teh itrem which to draw. keep the item given to remove it afterwards
     */
    void addItem(DrawItem* drawItem);

    /**
     * Remobves a shape from the image
     * @param drawItem the shape to remove
     */
    void removeItem(DrawItem* drawItem);

    /**
     * Removes all items of the given type #seeDrawType
     * @param drawType the type which shall be removed
     */
    void removeAll(DrawType drawType);

    /**
     * Removes all shapes from the iamges
     */
    void removeAllItems();

    /**
     * Sets the visibvle status of a group (true being visible)
     * @param groupNr the group for which to set the status
     * @param enable true shows all shapes, false hides them
     */
    void setGroupStatus(int groupNr, bool enable);

    /**
     * Removes all groups stati. If there are still shapes with group numbers
     * they will all be visible afterwards
     */
    void clearGroups();

    /**
     * Overwritten method paints the iamge given to the widget
     * @param event the Event which occured
     */
    void paintEvent(QPaintEvent* event);
protected:
    /**
     * Adds a QImage to the widget. Useful convinience method
     * @param image the image to be added
     */
    void addImage(QImage* image);

    /**
     * Adds all shapes to the image
     * @param shownImage teh iamge to add the shapes to
     */
    void addDrawItemsToWidget(QImage* shownImage);

    /** The image currently shown*/
    QImage* image;
    /** The format used in the widget*/
    QImage::Format format;
    /** The width of the widgets images*/
    int width;
    /** The height of the widgets images*/
    int height;
    /** List of all Draw Items*/
    QList<DrawItem*> items;
    /** List of group numbers currently disabled*/
    QList<int> disabledGroups;
};

#endif	/* IMAGEVIEWWIDGET_H */

