#ifndef MENUFORM_H
#define MENUFORM_H

#include <QObject>
#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QLabel>
#include "../forms/viewForm.h"

class MenuForm : public ViewForm
{
public:
   explicit MenuForm();
   void createUI()      override;
   void setupConnect()  override;
   void setupDisplay()  override;
   void setupButtons()  override;
   void setupLineEdit() override;
   void setupLayouts()  override;
   void setupLabel()    override;
   //QPushButton* createButton(const QString& text,StateButton state);

private:


};

#endif // MENUFORM_H
