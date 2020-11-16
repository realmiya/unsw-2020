import sys
from cspProblem import CSP, Constraint
from cspConsistency import Search_with_AC_from_CSP
# use Search_with_AC_from_CSP and make a class of search with CSP requirements of assignment1 to
# return a sum cost, then use GreedySearcher to
# return the solution(how to schedule tasks with smallest cost)
from searchGeneric import GreedySearcher
#in cspProblem.py we have class CSP,
# in Assign1, in order to "transfer" input of  input into dictionary of CSP,
# we need to add two more parameters into class SoftConstDict and SoftConstCostDict
# so make a TaskSchedulerCSP class
# TaskCsp = TaskSchedulerCSP(TaskDictionary, HardConstraintsList, SoftConstraintDict, SoftConstraintsCostDict)
# CSPInstance = Search_with_AC_from_Ass1_CSP(TaskCsp)#make an instance of problem
class TaskSchedulerCSP(CSP):
    def __init__(self, domains, Hardconstraints, SoftConstDict, SoftConstCostDict):
        """
        :param domains:a dictionary that maps each variable to its domain
        :param constraints:a list of hard constraints
        :param SoftConstDict: it is a dictionary, the format is {'t1': ['end-by    ']}
        :param SoftConstCostDict: it is a dictionary amd the format is {t1:10,...}
        """
        super().__init__(domains,Hardconstraints)
        #heritage from class CSP from cspProblem.py
        #then add two more parameters:
        self.SoftConstDict =SoftConstDict
        self.SoftConstCostDict = SoftConstCostDict


#To find a solution with Acr Consistency,
# we need Search_with_AC_from_CSP,we also need greedySearch and a new heuristic,
# then we can get the lowest cost
# so we make a new class Search_with_AC_from_Ass1_CSP
class Search_with_AC_from_Ass1_CSP(Search_with_AC_from_CSP):
    def __init__(self,csp):
        """
        :param csp: the CSP for schedule tasks
        """
        super().__init__(csp)
        #heritage from class Search_with_AC_from_CSP(Search_problem,Displayable) in cspConsistency.py
        self.ACSoftCons = csp.SoftConstDict
        self.ACSoftCostCons = csp.SoftConstCostDict
        #TaskSchedulerCSP and Search_with_AC_from_Ass1_CSP belong to the same big class CSP

    def heuristic(self, PossibleDomain):
        """
        :param PossibleDomain: the possible domain of each task, it is a dictionary
        :return: SumCost which is the sum of lowest cost of each task
        """
        ListOfCost = []
        for TaskNumber in PossibleDomain:
            if TaskNumber in self.ACSoftCons:
            #if in ACSoftCons has the tasknum:
                SoftDeadline = self.ACSoftCons[TaskNumber]
                TaskNumCostList = []
                for TaskDuration in PossibleDomain[TaskNumber]:
                    EndTime = TaskDuration[1]
                    if EndTime > SoftDeadline:
                    #if when task ends, it exceeds the soft deadline:
                        if (EndTime//10- SoftDeadline//10)>0:
                            #if it is not IN the same day, there is a 24*(number of days between soft cons and realEndday) delay
                            ExceedHour = (EndTime//10- SoftDeadline//10)*24 + ((EndTime%10) - (SoftDeadline%10))
                        else:
                            #if it is in the same day:
                            ExceedHour =((EndTime % 10) - (SoftDeadline % 10))
                        TaskNumCostList.append(self.ACSoftCostCons[TaskNumber] * ExceedHour)
                    else:
                        #if task has been condcucted within soft const:
                        #the cost==0, append 0 into TaskNumCostList
                        TaskNumCostList.append(0)
                if len(TaskNumCostList)!=0:
                    #there is a case of no solution, that is why need to judge len(TaskNumCostList)!=0:
                    #when there is no solution, no need to calculate cost
                    MinTaskCost=min(TaskNumCostList)
                    ListOfCost.append(MinTaskCost)
        SumCost = sum(ListOfCost)
        return SumCost



#hard domain constraints
def HardDay(weekday):
    """
    call the func of boolhardday(x)
    :param weekday: the value of weekday
    """
    def boolhardday(x):
        """
        judge whether the day in the domain of task is equal to the hard constraints for day
        return: Ture or False
        """
        return x[0] // 10 == weekday
    return boolhardday
    #return: Ture or False


def HardTime(time):
    """
    judge whether the time in the domain of task is equal to the time in the time in hard constraints
    :param time: the time of the task
    :return:Ture or False
    """
    boolhardtime=lambda x: x[0] % 10 == time
    return boolhardtime

def HardStartB4DayANDTime(day, time):
    """
    judge whether the day and time in the domain of task is start before that in the hard constraints
    :param day
    :param time
    :return:Ture or False
    """
    boolhardSb4DT=lambda value: value[0] <= day*10 + time
    return boolhardSb4DT

def HardStartB4Time(time):
    """
    judge whether the day and time in the domain of task is start before that in the  hard constraints
    :param time
    :return: Ture or False
    """
    boolhardSB4Time= lambda value:value[0]%10 <= time
    return boolhardSB4Time

def HStartAfterDayTime(day, time):
    """
    judge whether the day and time in the domain of task is start after that in the hard constraints
    :param day
    :param time
    :return: Ture or False
    """
    boolSADayTime=lambda value:value[0] >= time+day*10
    return boolSADayTime

def HStarAfterTime(time):
    """
    judge whether the time in the domain of task is start after that in the hard constraints
    :param time:
    :return:Ture or False
    """
    boolSATime= lambda x:x[0]%10 >= time
    return boolSATime

def HEndB4DT(day, time):
    """
    judge whether the day and time in the domain of task is end before that in the hard constraints
    :param day
    :param time
    :return: Ture or False
    """
    boolEndb4DT= lambda x:x[1] <= day*10 + time
    return boolEndb4DT

def HEndB4Time(time):
    """
    judge whether the time in the domain of task is end before that in the hard constraints
    :param time
    :return: Ture or False
    """
    boolHEndB4Time=lambda x:x[1]%10 <= time
    return boolHEndB4Time

def HEndAfterDT(day, time):
    """
    judge whether the day and time in the domain of task is end after that in the hard constraints
    :param day
    :param time
    :return: Ture or False
    """
    boolEndAfterDT=lambda x:x[1] >= (day*10 + time)
    return boolEndAfterDT

def HEndAfterTime(time):
    """
    judge whether the time in the domain of task is end after that in the hard constraints
    :param time
    :return: Ture or False
    """
    boolEndAfterT=lambda x:x[1]%10 >= time
    return boolEndAfterT

def HStartRange(D1, time1, D2, time2):
    """
    judge whether the start day and start time in the domain of certain task is
    in the range of D1, time1 to D2, time2 in the hard constraints
    :param D1: the start of day range
    :param time1: the start of time range
    :param D2: the end of day range
    :param time2: the end of time range
    :return: T or F
    """
    boolHStartRange=lambda x: x[0] >= (D1 * 10 + time1) and x[0] <= (D2 * 10 + time2)
    return boolHStartRange

def HEndRange(D1, time1, D2, time2):
    """
    judge whether the end day and end time in the domain of certain task is
    in the range of D1, time1 to D2, time2 in the hard constraints
    :param D1: the start of day range
    :param time1: the start of time range
    :param D2: the end of day range
    :param time2: the end of time range
    :return: T or F
    """
    boolEndRange=lambda x: x[1] >= (D1 * 10 + time1) and x[1] <= (D2 * 10 + time2)
    return boolEndRange


# A Constraint consists of* scope: a tuple of variables and
# * condition: a function that can applied to a tuple of values for the variables
# so a function of binary constraint need to be written below
def ConsSameDay(t1, t2):
    """
    judge whether t1 and t2 is in the same day
    :param t1
    :param t2
    :return:T or F
    """
    weekday1=t1[0]
    weekday2=t2[0]
    return bool(weekday1//10 == weekday2//10)

def ConsAfter(t1, t2):
    """
    judge whether the start time of t1 is after or at the end time of t2
    :param t1
    :param t2
    :return:T or F
    """
    StartTime1=t1[0]
    EndTime2=t2[1]
    return bool(StartTime1>=EndTime2)

def ConsStartsAt(t1, t2):
    """
    judge whether the start time of t1 is at the end time of t2
    :param t1
    :param t2
    :return:T or F
    """
    StartTime1=t1[0]
    EndTime2=t2[1]
    return bool(StartTime1 == EndTime2)

def ConsBefore(t1, t2):
    """
    judge whether the end time of t1 is before or at the start time of t2
    :param t1
    :param t2
    :return:T or F
    """
    EndTime1=t1[1]
    StartTime2=t2[0]
    return bool(EndTime1<=StartTime2)


#read filename
filename = sys.argv[1]
WeekdayDict = {'mon': 1, 'tue': 2, 'wed': 3, 'thu': 4, 'fri': 5}
TimeDict = {'9am': 1, '10am': 2, '11am':3, '12pm': 4, '1pm': 5, '2pm': 6, '3pm': 7, '4pm': 8, '5pm':9}
domain =set()
# TimeValue is from 9am to 5pm, work from 8 hours per WeekdayValue, but 9 items for each weekdays of set of domain
for weekday in range(1, 6):
    # range vary from 12345, exclude 6
    for TimeValue in range(1, 10):
        # range vary from 123456789, exclude 10
        domain.add(weekday * 10 + TimeValue)
        #set of domain == {11,12,13,14,15,16,17,18,19,21,22,23,24,25,26,27,28,29,31,32,
        # 33,34,35,36,37,38,39,41,42,43,44,45,46,47,48,49,51,52,53,54,55,56,57,58,59}


HardConstraintsList = []
TaskDictionary = {}
#key is name(tasknum) of task, and value is a set
TaskAndDuration = {}
SoftConstraintDict = {}
SoftConstraintsCostDict = {}

file = open(filename,'r', encoding = 'utf-8')
for input in file:
    #if read '\n'(i.e. input which is full of space)
    # and comment starts with "#",ccontinue read the next input until read content about constraints
    if (input == '\n') or (input[0] == '#'):
        continue

    input = input.strip()
    #The strip() method removes any leading (spaces at the beginning)
    #and trailing (spaces at the end) characters (space is the default leading character to remove)
    input = input.replace('-', ' ')
    #incase the read the input like 'constraint, t1 same-WeekdayValue t2'   or 'constraint, t1 starts-at t2'
    input = input.replace(',', '')
    #get rid of the comma after task
    input = input.split(' ')

    if input[0]== 'task':
        #if read taska with name and duration:
        # i.e.input=="task, t1 4"===>"task, t1, 4"
        TaskAndDuration[input[1]] = int(input[2])
        CertainDaySchedule = set()
        duration = int(input[2])
        for TaskStartTime in domain:
            if TaskStartTime % 10 + duration <= 9:
            #if the task can be finished at or before 5pm(tem % 10 + duration <= 9),
            # the task can be arranged in the same day's schedule
                CertainDaySchedule.add(TaskStartTime)
        TaskDictionary[input[1]] = set((TaskStartTime, TaskStartTime + duration) for TaskStartTime in CertainDaySchedule)
        # will give result like {'t1': {(14, 17), (21, 24)},'t2': {(24, 26), (26, 28)}.....}until all domains((timeStart , TimeEnd)of all tasks has been added into the TaskDict}
        #print(TaskDictionary)

    # elif read binary constraints
    elif input[0]== 'constraint':
        t1 = input[1]
        t2 = input[-1]
        #print(t1)
        #eg: 'constraint, t1 before t2'
        # import from cspProblem.py ,we use class Constraint(object)
        # A Constraint consists of * scope: a tuple of variables and
        # * condition: a function that can applied to a tuple of values for the variables
        if 'before' in input:
            HardConstraintsList.append(Constraint((t1, t2), ConsBefore))
        if 'same' in input:
            HardConstraintsList.append(Constraint((t1, t2), ConsSameDay))
        if 'after' in input:
            HardConstraintsList.append(Constraint((t1, t2), ConsAfter))
        if 'starts' in input:
            HardConstraintsList.append(Constraint((t1, t2), ConsStartsAt))

    # elif read soft deadliine constraints
    #domain, <t>, ends, by, <WeekdayValue>, <TimeValue>, <cost>       # cost per hour of missing deadline
    elif (input[0] == 'domain') and (input[-2] in TimeDict):
        #soft deadline constaints is in the above format
        #print(input)
        #['domain', 't1', 'ends', 'by', 'mon', '3pm', '10']
        taskNum = input[1]
        #get value from dict of weekdict and timedict
        TimeValue = TimeDict[input[5]]
        WeekdayValue = WeekdayDict[input[4]]
        #WeekdayDict = {'mon': 1, 'tue': 2, 'wed': 3, 'thu': 4, 'fri': 5}
        #TimeDict = {'9am': 1, '10am': 2, '11am':3, '12pm': 4, '1pm': 5, '2pm': 6, '3pm': 7, '4pm': 8, '5pm':9}
        SoftConstraintsCostDict[taskNum]=int(input[6])
        SoftConstraintDict[taskNum]= WeekdayValue * 10 + TimeValue


    # else: if read hard domain constraints
    else:
        taskNum = input[1]
        #the input example is like follows
        #['domain', 't1', 'mon']
        #['domain', 't4', 'tue']
        if input[2] in WeekdayDict:
            WeekdayValue = WeekdayDict[input[2]]
            HardConstraintsList.append(Constraint((taskNum,), HardDay(WeekdayValue)))
        # domain, t TimeValue
        elif input[2] in TimeDict:
            TimeValue = TimeDict[input[2]]
            HardConstraintsList.append(Constraint((taskNum,), HardTime(TimeValue)))

        elif ('starts' in input) and ('before' in input):
        # two cases:
            if input[-2] in WeekdayDict:
                # if read 'domain <t> starts before <WeekdayValue> <TimeValue>'
                WeekdayValue = WeekdayDict[input[-2]]
                TimeValue = TimeDict[input[-1]]
                HardConstraintsList.append(Constraint((taskNum,), HardStartB4DayANDTime(WeekdayValue, TimeValue)))
            #domain, <t> starts-before <TimeValue>
            else:
                # if read 'domain <t> starts before <TimeValue>'
                TimeValue = TimeDict[input[-1]]
                HardConstraintsList.append(Constraint((taskNum,), HardStartB4Time(TimeValue)))

        elif ('starts' in input) and ('after' in input):
        #two cases, use if to divide:
            if input[-2] in WeekdayDict:
                #domain, t ,starts-after ,day ,time
                WeekdayValue = WeekdayDict[input[-2]]
                TimeValue = TimeDict[input[-1]]
                HardConstraintsList.append(Constraint((taskNum,), HStartAfterDayTime(WeekdayValue, TimeValue)))

            else:
                # if read'domain, t starts-after time'
                TimeValue = TimeDict[input[-1]]
                HardConstraintsList.append(Constraint((taskNum,), HStarAfterTime(TimeValue)))

        elif ('ends' in input) and ('before' in input):
            if input[-2] in WeekdayDict:
                # domain t ends before WeekdayValue TimeValue
                WeekdayValue = WeekdayDict[input[-2]]
                TimeValue = TimeDict[input[-1]]
                HardConstraintsList.append(Constraint((taskNum,), HEndB4DT(WeekdayValue, TimeValue)))
            else:
                # domain t ends before TimeValue
                TimeValue = TimeDict[input[-1]]
                HardConstraintsList.append(Constraint((taskNum,), HEndB4Time(TimeValue)))

        elif ('ends' in input) and ('after' in input):
            #two cases
            if input[-2] in WeekdayDict:
                # domain, t ends-after WeekdayValue TimeValue
                WeekdayValue = WeekdayDict[input[-2]]
                TimeValue = TimeDict[input[-1]]
                HardConstraintsList.append(Constraint((taskNum,), HEndAfterDT(WeekdayValue, TimeValue)))
            else:
                # domain, t ends-after TimeValue
                TimeValue = TimeDict[input[-1]]
                HardConstraintsList.append(Constraint((taskNum,), HEndAfterTime(TimeValue)))

        else:
            # in the case of starts-in or ends-in range of day1 to day2 and T1 toT2 range
            if 'ends' in input:                
                #when read inpu line like: 'domain, <t> ends-in <day> <time>-<day> <time>'
                D1 = WeekdayDict[input[4]]
                D2 = WeekdayDict[input[6]]
                T1 = TimeDict[input[5]]
                T2 = TimeDict[input[7]]
                HardConstraintsList.append(Constraint((taskNum,), HEndRange(D1, T1, D2, T2)))
            elif 'starts' in input:
                # when read 'domain, t starts-in WeekdayValue TimeValue-WeekdayValue TimeValue'
                D1 = WeekdayDict[input[4]]
                D2 = WeekdayDict[input[6]]
                T1 = TimeDict[input[5]]
                T2 = TimeDict[input[7]]
                HardConstraintsList.append(Constraint((taskNum,), HStartRange(D1, T1, D2, T2)))
                #incase it is uft-8 with bom, we need to give value separately 


TaskCsp = TaskSchedulerCSP(TaskDictionary, HardConstraintsList, SoftConstraintDict, SoftConstraintsCostDict)
#make an instance of csp in this task schedule assgnment
CSPInstance = Search_with_AC_from_Ass1_CSP(TaskCsp)
#make an instance of problem


ScheduleDomainDict = GreedySearcher(CSPInstance).search()
# in searchGeneric.py, add class GreedySearcher
#GreedySearcher is from searchGeneric.py
#the result is from path/__repr__, it is a dictionary
#ScheduleDomainDict is a dict of for the instance of csp

if bool(ScheduleDomainDict) is True:
#if we have a domain dict for scheduling tasks successfully:
    FinalSchedule = ScheduleDomainDict.end()
# end() is a func from searchProblem, under class Path
#  it is to return 'self.arc.to_node', i.e.get the final solution
#print(FinalSchedule)##
    for task in FinalSchedule:
        for weekday in WeekdayDict:
            WeekdayVal=list(FinalSchedule[task])[0][0]//10
            if WeekdayDict[weekday] ==WeekdayVal:
                #judge task in which weekday
                PrintWeekday = weekday
        for time in TimeDict:
            TimeVal=list(FinalSchedule[task])[0][0]%10
            if TimeDict[time] == TimeVal:
                #judge time ,% is mod
                PrintTime = time
        print(f'{task}:{PrintWeekday} {PrintTime}')
#in display.py:
# change max_display_level from 1 to 0 ,
# to avoid printing sentence like this '7 paths have been expanded and 6 paths remain in the frontier'
    print(f'cost:{CSPInstance.heuristic(FinalSchedule)}')
else:
    print('No solution')


