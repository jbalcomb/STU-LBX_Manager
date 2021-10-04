#ifndef ATS_CLI_MENU_H
#define ATS_CLI_MENU_H


#include <string>

#ifdef DOS
#endif
#ifdef LIN
#endif
#ifdef MAC
#endif
#ifdef WIN
#endif

#ifdef WIN32
#endif
#ifdef WINNT
#endif

struct s_MENU_ITEM {
    int menu_item_number;
    char * menu_item_label;
};

//using std::string;
//class MenuItem
//{
//public:
//    MenuItem(string cmd, string desc, boost::function<bool()> action):Command(cmd),
//                                                                      Description(desc),
//                                                                      Action(action)
//    {}
//    boost::function<bool()> GetAction() { return Action; }
//    string GetDescription() { return Description; }
//    string GetCommand() { return Command; }
//private:
//    string Command;
//    string Description;
//    boost::function<bool()> Action;
//}



// // https://stackoverflow.com/questions/55296835/console-application-menu-with-classes-c
// // For menus, I recommend using state tables (data driven software):
//
// // Define short hand for function pointer
//typedef void (*Menu_Processing_Function_Ptr)();
//
//struct Menu_Item
//{
//    unsigned int  number;
//    const char *  text;
//    Menu_Processing_Function_Ptr p_processing_function;
//};


#endif  // ATS_CLI_MENU_H
