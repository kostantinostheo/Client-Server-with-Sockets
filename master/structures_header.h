#ifndef STRUCTURES_HEADER_FILE
#define STRUCTURES_HEADER_FILE

#include <string>
#include <array>
#include <bits/c++config.h>

using namespace std;

class Country_list
{
public:  

    string country, date;
    Country_list * next;


    Country_list(string c , Country_list * n)
    {
        date = country = c;
        next = n;
    }
    ~Country_list()
    {
        if(next != nullptr)
            delete next;
    }
    
    void insertList(string c , Country_list ** c_tmp)
    {
        Country_list * neos = new Country_list(c ,this);
        (*c_tmp) = neos;
    }
    
    void insertCountryList(string c , Country_list ** c_tmp)
    {
        insertList(c, c_tmp);
    }
    
    int size()
    {
        Country_list * tmp = this;
        int count_nodes = 0;
        while(tmp != nullptr)
        {
            count_nodes++;
            tmp = tmp->next;
        }
        return count_nodes;
    }
    
    int countCountries()
    {
        return size();
    }
    
    int search(string c)
    {
        Country_list * tmp = this;
        while(tmp != nullptr)
        {
            if (tmp->country == c || tmp->date == c)
            {
                return 1;
            }
            tmp = tmp->next;
        }
        return 0;
    }
    
    void printList()
    {
        Country_list * tmp = this;
        while(tmp != nullptr)
        {
            cout << tmp->country << endl;
            tmp = tmp->next;
        }

    }
    string nodeRet()
    {
        Country_list * tmp = this;
        string t_s = tmp->country;

        tmp = tmp->next;
        return t_s;
    }
};


class Date_list
{
    Country_list * head;
    
public:
    
    Date_list(){ head = nullptr; }
    
    void insert(string v)
    {
        head = new Country_list(v, head);
    }
    
    ~Date_list(){ delete head; }
    
    Country_list * getHead(){ return head; }
};


class Chain_node
{
    
    public:
        string recordID;
        string treatment; //ENTER or EXIT
        string fname;
        string lname;
        string date;
        int age;
        string country;
        Chain_node * next;



        Chain_node(string r, string t , string f, string l, string dt, int a , string c)
        {
            recordID = r;
            treatment = t;
            fname = f;
            lname = l;
            date = dt;
            age = a;
            country = c;
            next = nullptr;
        }
        ~Chain_node()
        {
            if(next != nullptr)
                delete next;
        }
    };

    class Chain
    {
        
        public:   
        Chain_node * first;      
            Chain()
            {
                first = nullptr;
            }
            ~Chain()
            {
                delete first;
            }

            void insertListHorizontaly(string r, string t , string f, string l, string dt, int a , string c)
            {
                Chain_node * neos;
                neos = new Chain_node(r,t,f,l,dt,a,c);
                neos->next = first;
                first = neos;             

            }

            void printChain()
            {
                Chain_node * tmp = first;
                while (tmp != nullptr)
                {
                    cout << tmp->recordID << " " << tmp->treatment << " " << tmp->fname << " " << tmp->lname << " " << tmp->age << " " << tmp->date << endl;
                    tmp = tmp->next;
                }
            
            }
            
    };


    class Backbone_node //katheta
    {

        public:

            string disease;
            Chain orizontia_list;
            Backbone_node * next;

            Backbone_node(string d)
            {
                disease = d;
                next = nullptr;

            }
            ~Backbone_node()
            {
                if( next != nullptr )
                    delete next;
            }
    };

    
    
    class Backbone
    {

    public:

        Backbone_node * root;


        Backbone()
        {
            root = nullptr;
        }
        ~Backbone()
        {
            delete root;
        }

        Backbone_node * isExist(string disease)
        {
            Backbone_node *tmp = root;
            while (tmp != nullptr)
            {
                if(tmp->disease == disease)
                    return tmp;
                tmp=tmp->next;
            }
            return nullptr;
            
        }

        void insertListVerticaly(string r, string t , string f, string l, string d, int a , string c)
        {
            Backbone_node * neos;
            neos = new Backbone_node(d);
            neos->next = root;
            root = neos;             
            
        }

        void insert(string r, string t , string f, string l, string dt, string d, int a , string c)
        {
            Backbone_node * tmp;
            tmp = isExist(d);

            if(tmp!=nullptr) //disease already exists.
            {
                tmp->orizontia_list.insertListHorizontaly(r,t,f,l,dt,a,c);
            }
            else
            {
                insertListVerticaly(r,t,f,l,d,a,c);
                root->orizontia_list.insertListHorizontaly(r,t,f,l,dt,a,c);
            }
        }

        // Statistics
        void statistics(string date, string country,string desease, int &range020, int &range2140, int &range4160, int &range60p)
        {
            Backbone_node * bn = isExist(desease);
            
            Chain_node *cn;
            
            cn = bn == nullptr ? nullptr : bn->orizontia_list.first;
            
            range020 = range2140 = range4160 = range60p = 0;
            
            while (cn != nullptr) {

                if( cn->country == country && cn->date == date )
                {
                    if( cn->age <= 20 )
                        range020++;
                    else if( cn->age <= 21 )
                        range2140++;
                    else if( cn->age <= 41 )
                        range4160++;
                    else if( cn->age <= 61 )
                        range60p++;
                }
                
                cn = cn->next;
            }

        }
        
        ////////////////diseaseFrequencyGeneral/////////////
        void FrequencySpecific(int fd_w, string disease, string date1, string date2, string country)
        {
            //cout << disease << endl;
            if(root == nullptr)
                perror("error");

            int counter = 0; 
            Backbone_node * tmp = root;

            while(tmp != nullptr)
            {
                if(tmp->disease == disease)
                {
                    //to vrila latheta.
                    Chain_node * t_c = tmp->orizontia_list.first;
                    while(t_c != nullptr)
                    {
                        if (t_c->country == country && dateBounds(date1 , date2, t_c->date)==true && t_c->treatment == "ENTER")
                            counter++;
                          
                            
                        t_c = t_c->next;
                        
                    }break;
                    
                }
                tmp = tmp->next;
                
            }
            stringstream ss;
            ss << counter;

            string tmp_s = ss.str();
            int msg_size = tmp_s.length()+1;

            write(fd_w , &msg_size , sizeof(int));
            write(fd_w, tmp_s.c_str() , msg_size);
            
        }

        void Frequency(int f_w, Country_list * c_l, string disease, string date1, string date2)
        {
            Backbone_node * tmp = root;
            if(tmp == nullptr)
                perror("error");
            

            while(c_l != nullptr)
            {
                FrequencySpecific(f_w, disease, date1, date2, c_l->country);
                c_l = c_l->next;
            }
            
        }
        ////////////////diseaseFrequencyGeneral/////////////

        ////////////////Admissions/////////////
        void AdmissionsSpecific(int fd_w, string disease, string date1, string date2, string country)
        {
            if(root == nullptr)
                perror("error");

            int counter = 0; 
            Backbone_node * tmp = root;

            while(tmp != nullptr)
            {
                if(tmp->disease == disease)
                {
                    //to vrila latheta.
                    Chain_node * t_c = tmp->orizontia_list.first;
                    while(t_c != nullptr)
                    {
                        if (t_c->country == country && dateBounds(date1 , date2, t_c->date)==true && t_c->treatment == "ENTER")
                            counter++;
                          
                            
                        t_c = t_c->next;
                        
                    }break;
                    
                }
                tmp = tmp->next;
                
            }
            stringstream ss;
            ss << country << " " << counter;

            string tmp_s = ss.str();
            int msg_size = tmp_s.length()+1;

            write(fd_w , &msg_size , sizeof(int));
            write(fd_w, tmp_s.c_str() , msg_size);
            
        }

        void Admissions(int f_w, Country_list * c_l, string disease, string date1, string date2)
        {
            Backbone_node * tmp = root;
            if(tmp == nullptr)
                perror("error");
            

            while(c_l != nullptr)
            {
                AdmissionsSpecific(f_w, disease, date1, date2, c_l->country);
                c_l = c_l->next;
            }
            
        }
        ////////////////Admissions/////////////
        ////////////////Discharges/////////////
        void DischargesSpecific(int fd_w, string disease, string date1, string date2, string country)
        {
            if(root == nullptr)
                perror("error");

            int counter = 0; 
            Backbone_node * tmp = this->root;

            while(tmp != nullptr)
            {
                if(tmp->disease == disease)
                {
                    //to vrila latheta.
                    Chain_node * t_c = tmp->orizontia_list.first;
                    while(t_c != nullptr)
                    {
                        if (t_c->country == country && dateBounds(date1 , date2, t_c->date)==true && t_c->treatment == "EXIT")
                            counter++;
                          
                            
                        t_c = t_c->next;
                        
                    }break;
                    
                }
                tmp = tmp->next;
                
            }
            stringstream ss;
            ss << country << " " << counter;

            string tmp_s = ss.str();
            int msg_size = tmp_s.length()+1;

            write(fd_w , &msg_size , sizeof(int));
            write(fd_w, tmp_s.c_str() , msg_size);
            
        }

        void Discharges(int f_w, Country_list * c_l, string disease, string date1, string date2)
        {
            Backbone_node * tmp = root;
            if(tmp == nullptr)
                perror("error");
            

            while(c_l != nullptr)
            {
                DischargesSpecific(f_w, disease, date1, date2, c_l->country);
                c_l = c_l->next;
            }
            
        }
        ////////////////Discharges/////////////
        void topkAgeRanges(int fd_w, int k, string country, string disease, string date1 , string date2)
        {
             
            if(root == nullptr)
                perror("error");

            
            Backbone_node * tmp = this->root;
            int decades[7] = {0};
            string decadeslabel[7] = {"0-10 " , "10-20 " , "20-30 " , "30-40 " , "40-50 " , "50-60 " , "60+ "};
            int sum;
            
            while(tmp != nullptr)
            {
                if(tmp->disease == disease)
                {
                    
                    Chain_node * t_c = tmp->orizontia_list.first;
                    //to vrila latheta.
                    while(t_c != nullptr)
                    {
                        if (t_c->country == country && dateBounds(date1 , date2, t_c->date)==true && t_c->treatment == "ENTER")
                        {
                            
                            if(t_c->age <= 10)
                                decades[0]++;
                            else if(t_c->age <= 20)
                                decades[1]++;
                            else if(t_c->age <= 30)
                                decades[2]++;
                            else if(t_c->age <= 40)
                                decades[3]++;
                            else if(t_c->age <= 50)
                                decades[4]++;
                            else if(t_c->age <= 60)
                                decades[5]++;
                            else
                                decades[6]++;
                        }  
                        t_c = t_c->next;
                    }
                    //borw an kanw break??
                }
                tmp = tmp->next;
                
            }
            
            sum = decades[0] + decades[1]+decades[2]+decades[3] + decades[4]+decades[5]+decades[6];
            
            decades[0] = decades[0]/sum*100;
            decades[1] = decades[1]/sum*100;
            decades[2] = decades[2]/sum*100;
            decades[3] = decades[3]/sum*100;
            decades[4] = decades[4]/sum*100;
            decades[5] = decades[5]/sum*100;
            decades[6] = decades[6]/sum*100;
            int l = sizeof(decades)/sizeof(decades[0]);

            int i, j, max, temp;
            string temp2;
            for (i = 0; i < l; i++) 
            {
                max = i;
                for (j = i + 1; j < l+1; j++)
                    if (decades[j] > decades[max])
                    {
                        max = j;
                    }
                        
                temp = decades[i];
                temp2 = decadeslabel[i];

                decades[i] = decades[max];
                decadeslabel[i] = decadeslabel[max];

                decades[max] = temp;
                decadeslabel[max] = temp2;
            }

            
                //cout << decadeslabel[i] << decades[i] << "%" << endl;

            stringstream ss;
            for (i = 0; i < k; i++)
            {
                ss << decadeslabel[i] << " " <<  decades[i] << "%" << endl;
            }
            

            string tmp_s = ss.str();
            int msg_size = tmp_s.length()+1;

            write(fd_w , &msg_size , sizeof(int));
            write(fd_w, tmp_s.c_str() , msg_size);
            
            //cout << "20-30 " << decades[1] << "%" << endl;
        }

        void searchPatientRecord(string recordID , int fd_w)
        {
            //cout << "*" << recordID << "*" << endl;
            Backbone_node * tmp = root;
            if(root == nullptr)
                perror("error");
            
            string date_entry, date_exit , f_n, l_n, disease_tmp ;
            int age_tmp;
            bool isFound = false;

            while(tmp != nullptr)
            {
                Chain_node * c_n = tmp->orizontia_list.first;
                while(c_n != nullptr)
                {
                    if(c_n->recordID == recordID)
                    {
                        if(c_n->treatment == "ENTER")
                        {
                            date_entry = c_n->date;
                            f_n = c_n->fname;
                            l_n = c_n->lname;
                            age_tmp = c_n->age;
                            disease_tmp = tmp->disease;
                            isFound = true;
                    
                        }                            
                        else
                            date_exit = c_n->date;
                    }
                    c_n = c_n->next;
                }
                tmp = tmp->next;
            }
            if (date_exit.empty())
            {
                date_exit = "--";
            }
            
            stringstream ss;
            int notF = 0;
            int h;

            if(isFound == false)
                write(fd_w , &notF , sizeof(int));
            else
            {
                ss << recordID << " " << f_n << " " << l_n << " " << disease_tmp << " " << age_tmp << " " << date_entry << " " << date_exit; 
                string tmp_s = ss.str();
                int msg_size = tmp_s.length()+1;
                //cout << "msg_size: " << msg_size << endl;
                write(fd_w , &msg_size , sizeof(int));
                write(fd_w, tmp_s.c_str() , msg_size);
                //cout << tmp_s << endl;
            }
            

            
        }

        void print()
        {
            Backbone_node *tmp = root;
            while (tmp != nullptr)
            {
                //print chain
                tmp->orizontia_list.printChain();
                tmp = tmp->next; 

            }
            
        }
    };
    

    

        
        
    

#endif