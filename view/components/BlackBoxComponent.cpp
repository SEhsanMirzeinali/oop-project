//
// Created by Purple-Rose on 8/9/2025.
//

#include "BlackBoxComponent.h"
#include <QGraphicsScene>
#include <QPixmap>
#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <sstream>
BlackBoxComponent::BlackBoxComponent(QObject* parent) : GraphicComponent(parent) {
    static int capacitorCounter = 0;
    initComponentName("X", ++capacitorCounter);
    initComponentValue("X");

}

QGraphicsItem* BlackBoxComponent::create(QGraphicsScene* scene) {
    QPixmap XImage("../images/BlackBox.png");
    XImage = XImage.scaled(80, 40, Qt::IgnoreAspectRatio);
    graphicItem = scene->addPixmap(XImage);
    graphicItem->setData(0, QVariant::fromValue(this));

    nameLabel = scene->addText(componentName);
    valueLabel = scene->addText(componentValue);

    QFont font("Arial", 12, QFont::Bold); // فونت Arial با سایز 10 و حالت Bold
    //font.setItalic(true); // حالت ایتالیک (اختیاری)
    valueLabel->setFont(font);
    valueLabel->setZValue(20);
    nameLabel->setFont(font);
    nameLabel->setZValue(20);
    updateLabelPosition();

    updatePorts();
    return graphicItem;
}

void BlackBoxComponent::setPorts() {
    if (graphicItem) {
        QRectF rect = graphicItem->boundingRect();
        port1 = snapToGrid(graphicItem->mapToScene(rect.left(), rect.center().y()));
        port2 = snapToGrid(graphicItem->mapToScene(rect.right(), rect.center().y()));
    }
}
QString BlackBoxComponent::getComponentPrefix() const
{
    return "X";
}
std::vector<std::string> BlackBoxComponent:: replaceCircuitNodes(std::vector<std::string> net,std::string n1,std::string n2) {
    for (auto& line : net) {
        size_t posA = line.find(" A ");
        size_t posB = line.find(" B ");

        // جایگزینی A با ali
        while (posA != std::string::npos) {
            line.replace(posA + 1, 1, n1);
            posA = line.find(" A ");
        }

        // جایگزینی B با reza
        while (posB != std::string::npos) {
            line.replace(posB + 1, 1, n2);
            posB = line.find(" B ");
        }
    }

    for (auto& line : net) {
        std::cout << line << std::endl;
    }

    return net;
}

std::vector<std::string> BlackBoxComponent::getNetList(){return netList;}
void BlackBoxComponent::handleUnipolarNetList(std::vector<std::string> net) {
    if (net.size() < 2) {
        return; // اگر خطوط کافی نباشند، تابع را خاتمه دهید
    }

    // استخراج دو گره آخر (خط ماقبل آخر به A و خط آخر به B تبدیل می‌شود)
    std::string nodeA = net[net.size()-1]; // خط آخر -> B
    std::string nodeB = net[net.size()-2]; // خط ماقبل آخر -> A

    // ایجاد نگاشت جایگزینی
    std::unordered_map<std::string, std::string> replacements = {
        {nodeA, "A"},
        {nodeB, "B"},
        {"0", "0"} // گره زمین تغییر نمی‌کند
    };

    int x_counter = 1; // شمارنده برای نام‌های X1, X2, ...
    std::vector<std::string> result;

    // پردازش تمام خطوط به جز دو خط آخر
    for (size_t i = 0; i < net.size()-2; i++) {
        std::istringstream line_stream(net[i]);
        std::string token;
        std::vector<std::string> tokens;

        // تقسیم خط به توکن‌ها
        while (line_stream >> token) {
            tokens.push_back(token);
        }

        if (tokens.empty()) {
            continue;
        }

        // پردازش هر توکن در خط
        for (size_t j = 0; j < tokens.size(); j++) {
            if (j == 0) continue; // نام کامپوننت را تغییر ندهید
            if (j == 3) continue; // مقدار کامپوننت (توکن چهارم) را تغییر ندهید

            if (replacements.find(tokens[j]) != replacements.end()) {
                tokens[j] = replacements[tokens[j]];
            } else if (tokens[j] != "0") {
                std::string new_name = "X" + std::to_string(x_counter++);
                replacements[tokens[j]] = new_name;
                tokens[j] = new_name;
            }
        }

        // بازسازی خط
        std::string new_line;
        for (size_t j = 0; j < tokens.size(); j++) {
            if (j > 0) new_line += " ";
            new_line += tokens[j];
        }
        result.push_back(new_line);
    }
    for(int i=0 ; i<result.size() ; i++) {
        net[i].assign("x", net[i].at(1));
    }
    netList=result;
    // چاپ نتیجه
    for (const auto& line : netList) {
        std::cout << line << std::endl;
    }
}