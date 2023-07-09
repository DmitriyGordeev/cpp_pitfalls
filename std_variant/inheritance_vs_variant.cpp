#include <iostream>
#include <variant>
#include <memory>
#include <vector>
using std::cout;


class ILabel {
public:
    virtual ~ILabel() = default;
    virtual std::string buildHtml() const = 0;
};


class TextLabel : public ILabel {
public:
    TextLabel(const std::string& name) : m_Name(name) {}

    std::string buildHtml() const override {
        return "<p>" + m_Name + "</p>";
    }

protected:
    std::string m_Name;
};


class DateLabel : public ILabel {
public:
    DateLabel(const std::string& date) : m_Date(date) {}

    std::string buildHtml() const override {
        return "<p class=date>" + m_Date + "</p>";
    }

protected:
    std::string m_Date;
};


class IconLabel : public ILabel {
public:
    IconLabel(const std::string& name, const std::string& iconSrc) : m_Name(name), m_IconSrc(iconSrc) {}

    std::string buildHtml() const override {
        return "<img src=" + m_IconSrc  + ">" + m_Name + "</p>";
    }

protected:
    std::string m_Name;
    std::string m_IconSrc;
};




// ==============================================================================================
class VTextLabel {
public:
    std::string buildHtml() {
        return "<p>" + m_Name + "</p>";
    }

    std::string m_Name;
};

class VDateLabel {
public:
    std::string buildHtml() {
        return "<p class=date>" + m_Date + "</p>";
    }

    std::string m_Date;
};

class VIconLabel {
public:
    std::string buildHtml() {
        return "<img src=" + m_IconSrc  + ">" + m_Name + "</p>";
    }

    std::string m_Name;
    std::string m_IconSrc;
};

class HTMLBuilder {
public:
    std::string operator()(const VTextLabel& label) {
        return "<p>" + label.m_Name + "</p>";
    }

    std::string operator()(const VDateLabel& label) {
        return "<p class=date>" + label.m_Date + "</p>";
    }

    std::string operator()(const VIconLabel& label) {
        return "<img src=" + label.m_IconSrc  + ">" + label.m_Name + "</p>";
    }
};


int main() {

    // Using inheritance:
    std::vector<std::unique_ptr<ILabel>> labels;
    labels.emplace_back(std::make_unique<TextLabel>("John"));
    labels.emplace_back(std::make_unique<DateLabel>("01/09/2000"));
    labels.emplace_back(std::make_unique<IconLabel>("ImageName", "/path/to/img.jpg"));

    std::string html;
    for(auto& l : labels)
        html += l->buildHtml() + "\n";

    cout << "with inheritance:\n" << html << "\n";


    // -------------------------------------------------------------------------------
    // Using std::variant
    std::vector<std::variant<VTextLabel, VDateLabel, VIconLabel>> variant_labels;
    variant_labels.emplace_back(VTextLabel {"John"});
    variant_labels.emplace_back(VDateLabel {"01/09/2000"});
    variant_labels.emplace_back(VIconLabel {"ImageName", "/path/to/img.jpg"});

    // Option 1. Caller class
    std::string html_CallerClass;
    for(auto& l : variant_labels)
        html_CallerClass += std::visit(HTMLBuilder{}, l) + "\n";
    cout << "with variants (HTMLBuilder class):\n" << html_CallerClass << "\n";


    // Option 2. generic lambda
    auto gen_lambda = [](auto& label) {
        return label.buildHtml();
    };
    std::string html_GenericLambda;
    for(auto& l : variant_labels)
        html_GenericLambda += std::visit(gen_lambda, l) + "\n";
    cout << "with variants (generic lambda caller):\n" << html_GenericLambda << "\n";


    return 0;
}
