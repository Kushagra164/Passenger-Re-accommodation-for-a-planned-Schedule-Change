#define CITY_CODE_LENGTH 4

enum ActionCDs{NIL4,HK,PD};
enum ClassCDs{NIL5,FC,BC,PC,EC};

int getClassCode(string str){
    if(str=="FC") return 1;
    else if(str=="BC") return 2;
    else if(str=="PC") return 3;
    else if(str=="EC") return 4;
    else return 0;
}

string getClass(int x){
    if(x==1) return "FC";
    else if(x==2) return "BC";
    else if(x==3) return "PC";
    else if(x==4) return "EC";
    else return " ";
}

int getActionCode(string str){
    if(str=="HK") return 1;
    else if(str=="PD") return 2;
    else return 0;
}
class Journey{
private:
    int JourneyID;
public:
    ActionCDs ActionCD;
    ClassCDs ClassCD;
    char Src[CITY_CODE_LENGTH];
    char Dest[CITY_CODE_LENGTH];
    vector<int> flights;              //vector<InventoryID>

    Journey(int uuid, ActionCDs action_cd, ClassCDs cls_cd, char src[CITY_CODE_LENGTH], char dest[CITY_CODE_LENGTH]){
        JourneyID=uuid;
        ActionCD = action_cd;
        ClassCD = cls_cd;
        strncpy(Src,src,CITY_CODE_LENGTH);
        strncpy(Dest,dest,CITY_CODE_LENGTH);
    }
};
map<int,Journey*> journeyMap;             //map<JourneyID,Journey*>
map<int,int> journeyToPnrMap;             //map<JourneyID,PnrID>