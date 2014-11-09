#include "keysetvisitor.hpp"

#include "treeviewmodel.hpp"

using namespace kdb;

KeySetVisitor::KeySetVisitor() {}

void KeySetVisitor::visit(ConfigNode &node)
{
    Key key = node.getKey();

    if(key && key.isValid()){
//        qDebug() << "Appending key " << QString::fromStdString(key.getName());
        m_set.append(key.dup());
    }
    else if(!key){
//        qDebug() << "Key of node " << node->getName() << " is null";
    }
    else{
//        qDebug() << "Key of node " << node->getName() << " is invalid";
    }
}

void KeySetVisitor::visit(TreeViewModel *model)
{
//    qDebug() << "===============start Visit==============";
    foreach (ConfigNodePtr node, model->model())
    {
        node->accept(*this);
    }
//    qDebug() << "===============end Visit================";
}

KeySet KeySetVisitor::getKeySet()
{
    return m_set.dup();
}
