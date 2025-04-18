#include <iostream>
#include <vector>
#include <queue>
#include <ctime>
#include <algorithm>
#include <iomanip>
#include <limits>
#include <sstream>

#ifdef _WIN32
    #include <windows.h>
    void sleep_seconds(int seconds) { Sleep(seconds * 1000); }
#endif

using namespace std;

struct Task
{
    string name;
    int priority; 
    time_t startTime;
    time_t endTime;   
    int duration;

    Task(string n, int p, time_t start, time_t end) 
        : name(n), priority(p), startTime(start), endTime(end)
    {
        duration = difftime(endTime, startTime);
    }
};

struct CompareTask
{
    bool operator()(const Task& t1, const Task& t2) 
    {
        if(t1.startTime == t2.startTime)  return t1.duration > t2.duration;
        return t1.startTime > t2.startTime;
    }
};

class TaskScheduler 
{
private:
    vector<Task> taskList;

public:
    void addTask(const string& name, int priority, time_t start, time_t end) 
    {
        taskList.push_back(Task(name, priority, start, end));

        cout<<"Task added: "<<name <<" | Priority: "<<priorityLabel(priority) <<'\n';

        char* startStr = ctime(&start); 
        cout<<"Start Time: "<<startStr;

        char* endStr = ctime(&end);
        cout<<"End Time: "<< endStr;

        cout<<"Task duration: "<< difftime(end, start)<<" seconds\n";
        cout<<"Task added successfully.\n";
    }

    void deleteTask(const string& name)
     {
        auto taskToRemove = remove_if(taskList.begin(), taskList.end(),[&](const Task& t){return t.name == name;});

        if(taskToRemove != taskList.end()) 
        {
            taskList.erase(taskToRemove, taskList.end());
            cout<<"Task " <<name<<" deleted successfully.\n";
        } 
        else
        {
            cout<<"Task "<<name<<" not found.\n";
        }
    }

    void updateTask(const string& name, int newPriority, time_t newStart, time_t newEnd) {
        bool found = false;
        for(auto& task : taskList) 
        {
            if(task.name==name)
             {
                task.priority = newPriority;
                task.startTime = newStart;
                task.endTime = newEnd;
                task.duration = difftime(newEnd, newStart);
                found = true;
                cout <<"Task "<<name<<" updated.\n";
                cout <<"New Priority: "<<priorityLabel(newPriority)<<'\n';
                cout <<"New Start Time: "<< ctime(&newStart);
                cout <<"New End Time: "<< ctime(&newEnd);
                cout <<"New Duration: "<<task.duration<<" seconds\n";
                break;
            }
        }
        if(!found)
         {
            cout<<"Task "<<name<<" not found.\n";
        }
    }

    void listTasks() const
     {
        if(taskList.empty()) 
        {
            cout <<"No tasks scheduled.\n";
            return;
        }
        cout <<"\nScheduled Tasks:\n";
        cout <<"---------------------------------\n";
        for(const auto& task : taskList)
         {
            cout << task.name<<" | Priority: " << priorityLabel(task.priority)<<'\n';
            cout <<"Start Time: " <<ctime(&task.startTime);
            cout <<"End Time: " <<ctime(&task.endTime);
            cout <<"Duration: " <<task.duration<<" seconds\n";
            cout <<"---------------------------------\n";

        }
    }

    void runScheduler()
     {
        if(taskList.empty())
         {
            cout << "No  scheduled tasks to run.\n";
            return;
        }
        
        cout <<"\nStarting Task Execution:\n\n";

        priority_queue<Task, vector<Task>, CompareTask> taskQueue(taskList.begin(), taskList.end());

        while(!taskQueue.empty()) 
        {
            Task current = taskQueue.top();
            taskQueue.pop();

            time_t now = time(0);
            if(now < current.startTime) 
            {
                cout <<"Waiting for task: "<<current.name<< " to start...\n";
                sleep_seconds(difftime(current.startTime, now));
            }

            cout <<"Running: " << current.name << " | Priority: " << priorityLabel(current.priority)<< " | Started at: " << ctime(&current.startTime);

            sleep_seconds(current.duration);

            time_t endTime = time(0);
            cout << "Completed: " <<current.name<< " | Finished at: " << ctime(&endTime) << endl;
        }
        cout << "All tasks completed.\n";
    }

const vector<Task>& getTaskList() const 
{
        return taskList;
}

private:
    string priorityLabel(int p) const 
    {
        switch(p)
        {
            case 1: return "High";
            case 2: return "Medium";
            case 3: return "Low";
        }
    }
};

bool stringToTime(const string& timeStr, time_t& resultTime) 
{
    struct tm tm = {};
    istringstream ss(timeStr);
    ss>> get_time(&tm, "%Y-%m-%d %H:%M:%S");

    if(ss.fail()) return false;
    
    resultTime = mktime(&tm);
    return true;
}

void clearCin()
{
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(),'\n');
}

int main() 
{
    TaskScheduler scheduler;
    string command;

    while(true) 
    {
        cout<<"Enter command(add, delete, update, list, run, exit): ";
        cin>>command;
        clearCin();

        if(command=="add") 
        {
            int count;
            cout<<"How many tasks do you want to add? ";
            cin>>count;
            clearCin();

            while(count <= 0) 
            {
                cout <<"Invalid type of input of number of tasks. Please enter valid number of tasks: ";
                cin >> count;
                clearCin();
            }
            
            cout<<"Enter tasks details:\n";
            for(int i=0; i<count; i++)
            {
                string taskName;
                int priority;
                string startTime, endTime;

                cout<<"\nTask "<<(i + 1)<<" :\n";
                cout<<"Enter task name: ";
                getline(cin,taskName);
                
                cout<<"Enter priority (1-High, 2-Medium, 3-Low): ";
                cin>>priority;
                clearCin();
                while(priority<1 || priority>3) 
                {
                    cout<<"Invalid priority. Please enter priority again (1-High, 2-Medium, 3-Low): ";
                    cin>>priority;
                    clearCin();
                }

                time_t start, end;
                while (true)
                {
                    cout <<"Enter start time (YYYY-MM-DD HH:MM:SS): ";
                    getline(cin, startTime);

                    cout <<"Enter end time (YYYY-MM-DD HH:MM:SS): ";
                    getline(cin, endTime);

                    bool validFormat = true;

                    if(startTime.length()!= 19 || startTime[4]!= '-' || startTime[7]!= '-' || startTime[10]!= ' ' || startTime[13]!= ':' || startTime[16]!= ':')  validFormat = false;

                    if(endTime.length()!= 19 || endTime[4]!= '-' || endTime[7]!= '-' || endTime[10]!= ' ' || endTime[13]!= ':' || endTime[16]!= ':')  validFormat = false;

                    if(!validFormat)
                    {
                        cout <<"Invalid format. Please enter both times in YYYY-MM-DD HH:MM:SS format.\n";
                        continue;
                    }

                    if(!stringToTime(startTime, start)) 
                    {
                        cout <<"Could not parse start time. Please try again.\n";
                        continue;
                    }
                
                    if(!stringToTime(endTime, end)) 
                    {
                        cout <<"Could not parse end time. Please try again.\n";
                        continue;
                    }

                    if(difftime(end, start) <= 0) 
                    {
                        cout <<"End time must be after start time. Please re-enter both times.\n";
                        continue;
                    }

                    break;
                }

                scheduler.addTask(taskName, priority, start, end);
            }

        }
        else if(command == "delete") 
        {
            string name;
            cout <<"Enter task name to delete: ";
            getline(cin, name);

            scheduler.deleteTask(name);

        } 
        else if(command == "update")
        {
            string name;        
            cout <<"Enter task name to update: ";
            getline(cin, name);

            int newPriority= -1;
            time_t newStart= -1, newEnd= -1;
        
            string choice;
            cout <<"Do you want to update priority? (y/n): ";
            getline(cin, choice);

            if(choice=="y" || choice=="Y")
            {
                cout <<"Enter new priority (1-High, 2-Medium, 3-Low): ";
                cin >> newPriority;
                clearCin();
                while(newPriority < 1 || newPriority > 3) 
                {
                    cout << "Invalid priority. Please enter again (1-High, 2-Medium, 3-Low): ";
                    cin >> newPriority;
                    clearCin();
                }
            }

            cout <<"Do you want to update start time? (y/n): ";
            getline(cin, choice);

            if(choice=="y" || choice=="Y") 
            {
                string newStartTime;
                while (true) 
                {
                    cout <<"Enter new start time (YYYY-MM-DD HH:MM:SS): ";
                    getline(cin, newStartTime);

                    if(newStartTime.length()!= 19 || newStartTime[4]!= '-' || newStartTime[7]!= '-' || newStartTime[10]!= ' ' || newStartTime[13]!= ':' || newStartTime[16]!= ':') 
                    {
                        cout << "Invalid format. Please try again.\n";
                        continue;
                    }
                    if(!stringToTime(newStartTime, newStart)) 
                    {
                        cout << "Could not parse start time.\n";
                        continue;
                    }
                    break;
                }
            }
        
            cout << "Do you want to update end time? (y/n): ";
            getline(cin, choice);
            if(choice =="y" || choice =="Y")
            {
                string newEndTime;
                while (true)
                {
                    cout <<"Enter new end time (YYYY-MM-DD HH:MM:SS): ";
                    getline(cin, newEndTime);

                    if(newEndTime.length()!= 19 || newEndTime[4]!= '-' || newEndTime[7]!= '-' || newEndTime[10]!= ' ' || newEndTime[13]!= ':' || newEndTime[16]!= ':') 
                    {
                        cout <<"Invalid format. Please try again.\n";
                        continue;
                    }
                    if(!stringToTime(newEndTime, newEnd))
                    {
                        cout << "Could not parse end time.\n";
                        continue;
                    }
                    break;
                }
            }
        
            if(newStart!= -1 && newEnd!= -1 && difftime(newEnd, newStart)<= 0)
            {
                cout <<"Error: End time must be after start time. Update aborted.\n";
            } 
            else
            {
                for(const auto& task:scheduler.getTaskList()) {
                    if(task.name==name)
                     {
                        if(newPriority == -1) newPriority =task.priority;
                        if(newStart == -1) newStart = task.startTime;
                        if(newEnd == -1) newEnd = task.endTime;
                        break;
                    }
                }
                scheduler.updateTask(name, newPriority, newStart, newEnd);
            }
        }

        else if(command =="list") 
        {
            scheduler.listTasks();
        } 

        else if(command == "run")
        {
            scheduler.runScheduler();
        }

        else if(command == "exit") 
        {
            break;

        } 
        else 
        {
            cout <<"Unknown command. Please try again.\n";
        }
    }

    return 0;
}
