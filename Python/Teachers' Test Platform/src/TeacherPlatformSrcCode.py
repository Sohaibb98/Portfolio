from tkinter import *
from ScrollableFrame import *

import pyrebase

#Font Styles
LNameFont = 'TkSmallCaptionFont'
captionFont='Courier 9 italic'

#BackGround Color
HomeScreenBG='#203b61'
ActivitiesBG='#4a2c24'
HomeTableBG='grey'
HomeLNameHoverBG='#3c414a'
HomeLNameBG='black'
HomeActivitiesBtnBG='#691629'
HomeAddLessonBG='#203b61'
ActivitiyTypeBG='#192852'
SelectedActivityBG='#3250a6'
ActivityBG='#5b656b'

#Other Colors
HomeLNameBorder='white'
HomeLNameText='white'
HomeAddLessonBorder='#137a94'
HomeAddLessonText='white'
AddActivityBorder='#9e6151'
AddActivityText='white'
ActivityTitle='#d7e352'

#Activity Types
ActivityType =['MCQ','Match The Following','Fill In The Blank', 'Long Answer']

root = Tk()
root.configure(bg=HomeScreenBG)
curActivity = Frame(root, bg=ActivitiesBG)
HomeScreen = Frame(root,bg=HomeScreenBG)
#root.geometry("500x500")
BackKey = PhotoImage(file='src/backlogo50.png')
delImg = PhotoImage(file='src/del30.png')
BackKeyHover = PhotoImage(file='src/backlogo50Hov.png')
# CrossImg=PhotoImage(file='src/cross20.jpg')
RemoveImg=PhotoImage(file='src/remove20.png')
RemoveImg30=PhotoImage(file='src/remove30.png')
TickImg=PhotoImage(file='src/tick20.png')
TickImg30=PhotoImage(file='src/tick30.png')
edittImg30=PhotoImage(file='src/edit30.png')
noNetwork30=PhotoImage(file='src/nonetwork30.png')
noNetwork100=PhotoImage(file='src/nonet100.png')

BackBtn = Label(curActivity, image=BackKey)

# TableFrameSF = ScrolledFrame(HomeScreen, background=HomeTableBG, borderwidth = 7,relief='flat',scrollbars='vertical')
# TableFrameSF.pack(side="top", expand=1, fill="both")
# TableFrameSF.bind_arrow_keys(root)
# TableFrameSF.bind_scroll_wheel(root)
# TableFrame = TableFrameSF.display_widget(Frame)

TableOuterFrame=ScrollableFrame(HomeScreen,bg=HomeTableBG,bd=7,width='450')
TableFrame=TableOuterFrame.scrollable_frame
TableFrame.configure(bg=HomeTableBG)
TableOuterFrame.add_height=38


def TablePack():
    TableOuterFrame.pack(padx=20,pady=20,expand=True, fill=BOTH)


def LabelToEntry(event,ow,ipadx=0,ipady=0,padx=0,pady=0,arr=[]):
    t=event.widget.cget('text')
    if t in arr:
        t=''
    event.widget.pack_forget()
    ow.pack(ipadx=ipadx,ipady=ipady,padx=padx,pady=pady)
    ow.delete(0,END)
    # print(ow.cget('font'))
    if ow.winfo_class()=='Text':
        ow.insert(INSERT,t)
    else:
        ow.insert(0,t)

def EntryToLabel(event,ow,ipady=0,ipadx=0,padx=0,pady=0,changeFont=LNameFont):
    t=event.widget.get()
    # print(t)
    event.widget.pack_forget()
    ow.pack(ipadx=ipadx,ipady=ipady,padx=padx,pady=pady)
    f=captionFont
    if t != "Type Lesson Name":
        f=changeFont
    ow.configure(text=t,font=f)
    return t

def RemoveLesson(event,ow,Tf,LIndex):
    ow.destroy()
    if len(Tf.winfo_children())==0:
        Tf.pack_forget()
    del rootObject['Lessons'][LIndex]
    # print(rootObject)

    dbUpdate()

def LNameHoverIn(event,col):
    event.widget.configure(bg=HomeLNameHoverBG)

def LNameHoverOut(event,col):
    event.widget.configure(bg=HomeLNameBG)

def BackKeyHoverIn(event):
    event.widget.configure(image=BackKeyHover)

def BackKeyHoverOut(event):
    event.widget.configure(image=BackKey)

def LeaveFocus(e):
    e.widget.master.focus()
    e.widget.tag_delete('temp')

def BackToHomeScr(event,LessonFrame,LessonNActivities):
    curActivity.pack_forget()
    LessonFrame.destroy()
    HomeScreen.pack(fill=BOTH,padx=0,pady=0)
    root.configure(bg=HomeScreenBG)
    # print(rootObject)

    #for a in rootObject:
        # print(a)

    updatedNActivities=str(len(rootObject['Lessons'][LIndex]['Activities']))
    LessonNActivities.configure(text=updatedNActivities+' Activities')


def RemoveOption(event,optionFrame):
    OptionsFrame=optionFrame.master
    opFrameArr=OptionsFrame.winfo_children()
    radBtn= optionFrame.winfo_children()[0]
    myIndex=radBtn.cget('value')
    for i in range(myIndex,len(opFrameArr)):
        tRadBtn=opFrameArr[i].winfo_children()[0]
        tRadBtn.configure(value=tRadBtn.cget('value')-1)

    optionFrame.destroy()


def displayOption(optionsVar):
    # print('OPTION# '+ str(optionsVar.get()))
    pass


def addOption(event,parent,option,val,optionsVar,list,appended):
    if appended==False:
        list.append(option)
    # print(list)
    OptionBG='black'
    OptionText='white'
    OptionFrame=Frame(parent,bg=parent.cget('bg'))
    OptionFrame.pack(padx=7,pady=2)
    Radiobutton(OptionFrame,
                text='',
                padx=20,
                bg=OptionBG,
                variable=optionsVar,
                command=lambda: displayOption(optionsVar),
                value=val, fg='white',selectcolor=OptionBG).pack(side=LEFT,padx=8)


    OptionTextFrame=Frame(OptionFrame,bd=1,bg=OptionBG)
    font=captionFont
    newOptionCaption='Type Option Text'
    if option!=newOptionCaption:
        font=LNameFont

    OptionTextRead=Label(OptionTextFrame,text=option,width=20,bg=OptionBG,fg=OptionText,font=font)
#     #print('Before '+ OptionTextRead.cget('font'))
    OptionTextWrite=Entry(OptionTextFrame,width=20,bg=OptionBG,fg=OptionText,justify='center')

    OptionTextWrite.configure(insertbackground='white')

    OptionTextRead.bind("<Button-1>", lambda eff: LabelToEntry(eff, ow=OptionTextWrite,arr=[newOptionCaption]))
    OptionTextRead.bind("<Enter>", lambda eff: LNameHoverIn(eff,HomeLNameHoverBG))
    OptionTextRead.bind("<Leave>", lambda eff: LNameHoverOut(eff,HomeLNameBG))

    OptionTextWrite.bind("<Leave>", lambda eff: EntryToLabel(eff, ow=OptionTextRead,ipady=1,ipadx=5))
    OptionTextWrite.bind("<Return>", lambda eff: EntryToLabel(eff, ow=OptionTextRead,ipady=1,ipadx=5))


    OptionTextRead.pack(ipadx=5,ipady=1)
    OptionTextFrame.pack(side=LEFT,padx=6)


    #OptionTextRead.pack(side=LEFT,ipadx=3,ipady=3)
    DeleteOption=Label(OptionFrame,image=RemoveImg,bg='#5b656b',cursor='hand2')
    DeleteOption.pack(ipadx=3,ipady=3,padx=15)
    DeleteOption.bind("<Button-1>",lambda eff: RemoveOption(eff,OptionFrame))

def setMCQ(ActivityFrame,curActivityStatus,val,selectedVal):
    MCQObject = {}
    MCQFrame = {}
    newQuestionCaption='Type your Question here'
    if val != -1 and ActType(val)=='MCQ':
        # print('At 1')
        MCQObject = val
    else:
        # print('At 2')
        MCQObject = {"Question": newQuestionCaption, "Options": ['Option A'], 'Answer':0}

    if 'MCQ' not in curActivityStatus:
        # print('At 3')
        optionsVar = IntVar()
        optionsVar.set(MCQObject['Answer'])
        curActivityStatus['MCQ Variable']=optionsVar
        MCQFrame = Frame(ActivityFrame, bg=ActivityBG)

        QuestionTextFrame = Frame(MCQFrame, bd=0)
        QuestionTextRead = Label(QuestionTextFrame, text=MCQObject['Question'], cursor='ibeam', width=40)

        QuestionTextWrite = Entry(QuestionTextFrame, width=40, justify='center')

        QuestionTextRead.bind("<Button-1>", lambda eff: LabelToEntry(eff, ow=QuestionTextWrite,arr=[newQuestionCaption]))

        QuestionTextWrite.bind("<Leave>", lambda eff: EntryToLabel(eff, ow=QuestionTextRead,ipadx=5))
        QuestionTextWrite.bind("<Return>", lambda eff: EntryToLabel(eff, ow=QuestionTextRead,ipadx=5))

        QuestionTextRead.pack(ipadx=5, ipady=1)

        QuestionTextRead.pack()
        QuestionTextFrame.pack(pady=4)

        OptionsFrame = Frame(MCQFrame, bg='#5b656b')

        for val, option in enumerate(MCQObject['Options']):
            addOption(0, OptionsFrame, option, val, optionsVar, MCQObject['Options'], True)

        AddOptionBtnBorder = Frame(MCQFrame, bg=HomeAddLessonBorder, borderwidth=3)
        AddOptionBtn = Button(AddOptionBtnBorder, bg=HomeAddLessonBG, bd=0, text='+ Add Option', cursor='hand2',
                              fg=AddActivityText)
        OptionsFrame.pack(pady=6)
        AddOptionBtn.bind("<Button-1>",
                          lambda eff: addOption(eff, OptionsFrame, "Type Option Text", len(OptionsFrame.winfo_children()),
                                                optionsVar,
                                                MCQObject['Options'], False))
        AddOptionBtn.pack(ipadx=5)
        AddOptionBtnBorder.pack(pady=20)

    else:
        # print('At 4')
        MCQFrame = curActivityStatus['MCQ']

    MCQFrame.pack(padx=40, pady=10, side=LEFT)

    curActivityStatus['MCQ'] = MCQFrame

def addBlankBox(startInd,w,BlanksFrame):
    endInd=str(w.tag_ranges(startInd)[1])
    word=w.get(startInd,endInd)
    BlankFrame=Frame(BlanksFrame)
    Blank=Label(BlankFrame,text=word)
    remove=Label(BlankFrame,image=RemoveImg)

    Indexes=Label(BlankFrame,text=startInd+','+endInd)
    Blank.pack(side=LEFT)
    remove.pack()
    count=len(BlanksFrame.winfo_children())
    perRowBoxes=3
    i=0
    column=0
    row=0
    while i>=0:
        for j in range(perRowBoxes):
            if len(BlanksFrame.grid_slaves(row=i,column=j))==0:
                row=i
                column=j
                i=-2
                break
        i+=1

    BlankFrame.grid(row=row,column=column, padx=8,pady=3)
    Indexes.pack_forget()

def HighlightWord(e,w=None):
    CharactersInOneWord=[]
    for i in range(65,91):
        CharactersInOneWord.append(chr(i))
    for i in range (97,123):
        CharactersInOneWord.append(chr(i))
    for i in range(48,58):
        CharactersInOneWord.append(chr(i))
    CharactersInOneWord.append('-')
    CharactersInOneWord.append('\'')
    if w == None:
        w=e.widget
    lineind=int(w.index(CURRENT)[0])
    line=str(lineind)+'.0'
    index=int(w.index(CURRENT)[2:])
#     #print('cursor: '+str(index))
    start=index
    while w.get('%d.%d' % (lineind,start)) in CharactersInOneWord and start>=0:
        start -= 1
    start+=1
    end=index
    lastChar = int(w.index(line+'+1l-1c')[2:])
    while w.get('%d.%d' % (lineind,end)) in CharactersInOneWord and end < lastChar:
        end += 1
    w.tag_delete('temp')
#     #print('s: '+str(start)+', e: '+str(end))
#     #print('last: '+str(lastChar))
    if end>lastChar:
        end=lastChar
#     #print('end after: '+str(end))
    if end>start and index<lastChar:
        StartIndStr=line[:2]+str(start)
        EndIndStr=line[:2]+str(end)
#         #print ('start: '+StartIndStr+', end: '+EndIndStr)
        if StartIndStr not in w.tag_names():
            w.tag_add('temp',StartIndStr,EndIndStr)
            w.tag_config('temp',background=HomeLNameHoverBG,foreground='white')

def SelectHighlighted(event,w=None):
    if w==None:
        w=event.widget
#     #print(w.tag_names())
    for tag in w.tag_names():
#         #print(tag+': ')
        startInd=''
        if tag!='sel':
            startInd=str(w.tag_ranges(tag)[0])
#             #print(startInd+', '+str(w.tag_ranges(tag)[1]))
        if tag=='temp':
            w.tag_add(startInd,startInd,str(w.tag_ranges(tag)[1]))
            w.tag_config(startInd,background='white',foreground='black')
            w.tag_delete('temp')

            BlanksFrame=w.master.master.winfo_children()[1]
            addBlankBox(startInd,w,BlanksFrame)

def tempHighlight(e,TextWid):
    Indexes=e.widget.master.winfo_children()[2].cget('text')
    IndArr=Indexes.split(sep=',')
    TextWid.tag_add('temp',IndArr[0],IndArr[1])
    TextWid.tag_config('temp',background=HomeLNameHoverBG,foreground='white')

def tempUnHighlight(e,TextWid):
    TextWid.tag_delete('temp')

def removeBlank(e,t):
    e.widget.master.destroy()
    t.tag_delete('temp')

def DoneAddingBlanks(e,AddBlankBtnBorder,TextWid,w=None):
    TextWid.configure(cursor='ibeam',state='normal')
    TextWid.unbind("<Motion>")
    TextWid.unbind("<Button-1>")
    if w==None:
        e.widget.pack_forget()
    else:
        w.pack_forget()
    AddBlankBtnBorder.pack()

    for tag in TextWid.tag_names():
        TextWid.tag_delete(tag)

    BlanksFrame= TextWid.master.master.winfo_children()[1]
    BFChilds=BlanksFrame.winfo_children()

    for j in range(len(BFChilds)):
        subFs = BFChilds[j].winfo_children()
        for i in range(len(subFs)):
#             # print(BF.winfo_children()[2].cget('text'))
            if (i<2):
                subFs[i].bind("<Enter>",lambda e:tempHighlight(e,TextWid))
                subFs[i].bind("<Leave>",lambda e:tempUnHighlight(e,TextWid))
            subFs[1].configure(cursor='hand2')
            subFs[1].bind("<Button-1>",lambda e: removeBlank(e,TextWid))

def AddBlank(e, DoneBtn,TextWid):
    TextWid.configure(cursor='arrow',state='disabled')
    TextWid.bind("<Motion>",HighlightWord)
    TextWid.bind("<Button-1>",SelectHighlighted)
    e.widget.master.pack_forget()
    DoneBtn.pack()

    BlanksFrame= TextWid.master.master.winfo_children()[1]
    for BF in BlanksFrame.winfo_children():
        # print(BF)
        for i,subFs in enumerate(BF.winfo_children()):
            subFs.unbind("<Enter>")
            subFs.unbind("<Leave>")

            if i==2:
                IndArr = subFs.cget('text').split(sep=',')
                TextWid.tag_add(IndArr[0], IndArr[0], IndArr[1])
                TextWid.tag_config(IndArr[0], background='white', foreground='black')
            elif i==1:
                subFs.configure(cursor='arrow')
                subFs.unbind("<Button-1>")

def TextEdited(e,text, BlanksFrame):
    if e.widget.get('1.0')!=text:
        text=e.widget.get('1.0')
        for BF in BlanksFrame.winfo_children():
            BF.destroy()

def setFITB(ActivityFrame,curActivityStatus,val,selectedVal):
    string=ActivityType[selectedVal]
    FITBObject = {}
    FITBFrame = {}
    newSentenceCaption='Type Your Sentence Here'
    if val != -1 and ActType(val)=='FITB':
        # print('At 1 FITB')
        FITBObject = val
        if 'Blanks' not in FITBObject:
            FITBObject["Blanks"]=[]
    else:
        # print('At 2')
        FITBObject = {"Sentence": "Type your Sentence here", "Blanks": ['1.0','1.10']}

    if string not in curActivityStatus:
        # print('At 3')

        FITBFrame = Frame(ActivityFrame, bg=ActivityBG)

        QuestionTextFrame = Frame(FITBFrame, bd=0)

        QuestionTextWrite = Text(QuestionTextFrame, width=70,bg='black',fg='white',height=3)
        QuestionTextWrite.configure(insertbackground='white')


        QuestionTextWrite.bind("<Leave>", lambda e: LeaveFocus(e))

        QuestionTextWrite.pack()
        QuestionTextFrame.pack(pady=4,padx=6)

        BlanksFrame=Frame(FITBFrame,bg=ActivityBG)
        BlanksFrame.pack()

        BtnFrame=Frame(FITBFrame,bd=0)
        Donebtn=Label(BtnFrame,image=TickImg, cursor='hand2')
        AddBlankBtnBorder = Frame(BtnFrame, bg=HomeAddLessonBorder, borderwidth=3)
        AddBlankBtn = Button(AddBlankBtnBorder, bg=HomeAddLessonBG, bd=0, text='+ Add Blank', cursor='hand2',
                              fg=AddActivityText)

        AddBlankBtn.bind("<Button-1>",lambda e:AddBlank(e,Donebtn,QuestionTextWrite))
        Donebtn.bind("<Button-1>",lambda e: DoneAddingBlanks(e,AddBlankBtnBorder,QuestionTextWrite))


        AddBlankBtn.pack(ipadx=5)
        AddBlankBtnBorder.pack()
        BtnFrame.pack(pady=20)


        QuestionTextWrite.insert(INSERT,FITBObject['Sentence'])
        text=FITBObject['Sentence']
        for val,blank in enumerate(FITBObject['Blanks']):
            # print (blank)
            QuestionTextWrite.mark_set('current',blank.split(',')[0])
            HighlightWord(None,QuestionTextWrite)
            SelectHighlighted(None, QuestionTextWrite)
        DoneAddingBlanks(None,AddBlankBtnBorder,QuestionTextWrite,Donebtn)

        QuestionTextWrite.bind("<KeyRelease>",lambda e:TextEdited(e,text,BlanksFrame))

    else:
        # print('At 4')
        FITBFrame = curActivityStatus[string]

    FITBFrame.pack(padx=40, pady=10, side=LEFT)

    curActivityStatus[string] = FITBFrame

def addPairBox(PairFrame,A,A_BG,A_Text, cfont= None,ipadx=5,ipady=0):
#     #print('font: '+font+' at 488')
    AFrame=Frame(PairFrame,bd=1,bg=A_BG)
    pairCaptions=['Left Side','Right Side']
    font=captionFont
    if A not in pairCaptions:
        font=LNameFont

    ATextRead=Label(AFrame,text=A,width=30,bg=A_BG,fg=A_Text,font=font if cfont==None else cfont)
#     #print('Before '+ OptionTextRead.cget('font'))
    ATextWrite=Entry(AFrame,width=30,bg=A_BG,fg=A_Text,justify='center',font=font if cfont==None else cfont)

    ATextWrite.configure(insertbackground='white')

    ATextRead.bind("<Button-1>", lambda eff: LabelToEntry(eff, ow=ATextWrite,arr=pairCaptions,ipady=ipady))
    ATextRead.bind("<Enter>", lambda eff: LNameHoverIn(eff,HomeLNameHoverBG))
    ATextRead.bind("<Leave>", lambda eff: LNameHoverOut(eff,HomeLNameBG))


    if cfont!=None:
        ATextWrite.bind("<Leave>", lambda eff: EntryToLabel(eff, ow=ATextRead, ipady=1, ipadx=ipadx,changeFont=cfont))
        ATextWrite.bind("<Return>", lambda eff: EntryToLabel(eff, ow=ATextRead, ipady=1, ipadx=ipadx,changeFont=cfont))
    else:
        ATextWrite.bind("<Leave>", lambda eff: EntryToLabel(eff, ow=ATextRead, ipady=1, ipadx=ipadx))
        ATextWrite.bind("<Return>", lambda eff: EntryToLabel(eff, ow=ATextRead, ipady=1, ipadx=ipadx))

    ATextRead.pack(ipadx=ipadx,ipady=1)
    #AFrame.pack(side=LEFT,padx=12)
    return AFrame

def RemovePair(eff,PairFrame):
    PairFrame.destroy()

def addPair(event,parent,obj,A='',B='',appended=False):
    if appended==False:
        obj['A'].append(A)
        obj['B'].append(B)
    # print(obj)
    A_BG='black'
    A_Text='white'
    B_BG = 'black'
    B_Text = 'white'
    PairFrame=Frame(parent,bg=parent.cget('bg'))
    PairFrame.pack(padx=3,pady=5)

    AFrame=addPairBox(PairFrame,A,A_BG,A_Text)
    AFrame.pack(side=LEFT, padx=12)
    BFrame=addPairBox(PairFrame,B,B_BG,B_Text)
    BFrame.pack(side=LEFT, padx=12)

    #OptionTextRead.pack(side=LEFT,ipadx=3,ipady=3)
    DeleteOption=Label(PairFrame,image=RemoveImg,bg='#5b656b',cursor='hand2')
    DeleteOption.pack(ipadx=3,ipady=3,padx=15)
    DeleteOption.bind("<Button-1>",lambda eff: RemovePair(eff,PairFrame))

def ActType(object):
    if 'Options' in object:
        return "MCQ"
    if 'A' in object:
        return 'MTF'
    if 'Sentence' in object:
        return 'FITB'
    if 'Answer' in object:
        return 'Long Answer'

def setMTF(ActivityFrame,curActivityStatus,val,selectedVal):
    string=ActivityType[selectedVal]
    MTFObject = {}
    MTFFrame = {}
    if val != -1 and ActType(val)=='MTF':
        # print('At 1MTF')
        # print(val)
        MTFObject = val
    else:
        # print('At 2MTF')
        MTFObject = {"A": ['Statement First Half'], "B": ['Statement Second Half']}

    if string not in curActivityStatus:
        # print('At 3MTF')
        MTFFrame = Frame(ActivityFrame, bg=ActivityBG)

        PairsFrame = Frame(MTFFrame, bg='#5b656b')
        PairsFrame.pack(pady=8)

        #for val, option in enumerate(MTFObject['A']):
            #addOption(0, OptionsFrame, option, val, optionsVar, MTFObject['Options'], True)

        for val,A in enumerate(MTFObject['A']):
            addPair(0, PairsFrame, MTFObject, A, MTFObject['B'][val], True)

        AddPairBtnBorder = Frame(MTFFrame, bg=HomeAddLessonBorder, borderwidth=3)
        AddPairBtn = Button(AddPairBtnBorder, bg=HomeAddLessonBG, bd=0, text='+ Add Pair', cursor='hand2',
                              fg=AddActivityText)

        AddPairBtn.bind("<Button-1>",
                          lambda eff: addPair(eff, PairsFrame, MTFObject,
                                                'Left Side','Right Side', False))
        AddPairBtn.pack(ipadx=5)
        AddPairBtnBorder.pack(pady=20,padx=20)

    else:
        # print('At 4MTF')
        MTFFrame = curActivityStatus[string]

    MTFFrame.pack(padx=40, pady=10, side=LEFT)

    curActivityStatus[string] = MTFFrame

def setLA(ActivityFrame,curActivityStatus,val,selectedVal):
    string=ActivityType[selectedVal]
    LAObject = {}
    LAFrame = {}
    newSentenceCaption='Type Your Question Here'
    if val != -1 and ActType(val)=='Long Answer':
        # print('At 1 LA')
        LAObject = val
    else:
        # print('At 2 LA')
        LAObject = {"Question": "Type your Question here", "Answer": 'Type the Answer here'}

    if string not in curActivityStatus:
        # print('At 3 LA')

        LAFrame = Frame(ActivityFrame, bg=ActivityBG)

        QuestionTextFrame = Frame(LAFrame, bd=0)

        QuestionTextWrite = Text(QuestionTextFrame, width=50,bg='black',fg='white',height=2)
        QuestionTextWrite.configure(insertbackground='white')


        QuestionTextWrite.bind("<Leave>", lambda e: LeaveFocus(e))

        QuestionTextWrite.pack()
        QuestionTextFrame.pack(pady=4,padx=6)

        QuestionTextWrite.insert(INSERT, LAObject['Question'])


        AnswerTextFrame = Frame(LAFrame, bd=0)

        AnswerTextWrite = Text(AnswerTextFrame, width=50, bg='white', fg='black', height=3)
        #QuestionTextWrite.configure(insertbackground='white')

        AnswerTextWrite.bind("<Leave>", lambda e: LeaveFocus(e))

        AnswerTextWrite.pack()
        AnswerTextFrame.pack(pady=4, padx=6)


        AnswerTextWrite.insert(INSERT,LAObject['Answer'])
        text=LAObject['Answer']



    else:
        # print('At 4 LA')
        LAFrame = curActivityStatus[string]

    LAFrame.pack(padx=40, pady=10, side=LEFT)

    curActivityStatus[string] = LAFrame

def saveLA(actFrame):
    ActivityUpdatedObject = {'Question': '', 'Answer': ''}
    QuestionWidget = actFrame.winfo_children()[0].winfo_children()[0]
    AnswerWidget = actFrame.winfo_children()[1].winfo_children()[0]
    ActivityUpdatedObject['Question']=QuestionWidget.get('1.0','end-1c')
    ActivityUpdatedObject['Answer']=AnswerWidget.get('1.0','end-1c')

    # print(ActivityUpdatedObject)

    return ActivityUpdatedObject

def saveMTF(actFrame):
    ActivityUpdatedObject = {'A': [], 'B': []}
    PairsFrame = actFrame.winfo_children()[0]
    for pair in PairsFrame.winfo_children():
        Aframe = pair.winfo_children()[0]
        AText = Aframe.winfo_children()[0].cget('text')
        ActivityUpdatedObject['A'].append(AText)

        Bframe = pair.winfo_children()[1]
        BText = Bframe.winfo_children()[0].cget('text')
        BText = Bframe.winfo_children()[0].cget('text')
        ActivityUpdatedObject['B'].append(BText)
    # print(ActivityUpdatedObject)

    return ActivityUpdatedObject

def saveFITB(actFrame):
    ActivityUpdatedObject = {'Sentence': '', 'Blanks': []}
    QuestionTextFrame=actFrame.winfo_children()[0]
    QuestionWidget=QuestionTextFrame.winfo_children()[0]
    ActivityUpdatedObject['Sentence']=QuestionWidget.get('1.0','end-1c')

    BlanksFrame=actFrame.winfo_children()[1]
    for BF in BlanksFrame.winfo_children():
        BlankIndexesLabel = BF.winfo_children()[2]
        Indexes=BlankIndexesLabel.cget('text')
        ActivityUpdatedObject['Blanks'].append(Indexes)

    # print(ActivityUpdatedObject)

    return ActivityUpdatedObject


def saveMCQ(actFrame, optionsVar):

    QuestionFrame=actFrame.winfo_children()[0]
    QuestionWidget=QuestionFrame.winfo_children()[0]
    Question=QuestionWidget.cget('text')

    ActivityUpdatedObject = {'Question': Question, 'Options': [], 'Answer': 0}

    OptionsFrame=actFrame.winfo_children()[1]
    for OF in OptionsFrame.winfo_children():
        OpTextFrame=OF.winfo_children()[1]
        OpTextWidget=OpTextFrame.winfo_children()[0]
        Option=OpTextWidget.cget('text')
        ActivityUpdatedObject['Options'].append(Option)

    ActivityUpdatedObject['Answer']=optionsVar.get()

    # print(ActivityUpdatedObject)

    return ActivityUpdatedObject

def EditActivity(e):
    e.widget.master.pack_forget()
    e.widget.master.master.winfo_children()[1].pack(pady=25)

def CollapseActivity(ActivityFrame,Type,AIndex,actModified=True):
    ActivityFrame.pack_forget()
    ActivityOuter=ActivityFrame.master
    activityCollapsed=ActivityOuter.winfo_children()[0]
    activityCollapsed.pack()
    activityNameLabel=activityCollapsed.winfo_children()[0].winfo_children()[0]
    activityName=activityNameLabel.cget('text')
    if activityName=='' or actModified==True:
        #for key in rootObject['Lessons'][LIndex]['Activities'][AIndex]:
        #     print ('key: '+key)
        # print(rootObject['Lessons'][LIndex]['Activities'][AIndex])
        if 'Name' in rootObject['Lessons'][LIndex]['Activities'][AIndex]:
            activityName=rootObject['Lessons'][LIndex]['Activities'][AIndex]['Name']
        else:
            ActivitiesFrame=ActivityOuter.master
            i=0
            for activity in ActivitiesFrame.winfo_children():
                if activity == ActivityOuter:
                    break
                i+=1
            # print ('i= '+str(i))
            activityName='Activity '+ str(i+1)+': '+ Type
            rootObject['Lessons'][LIndex]['Activities'][AIndex]['Name']=activityName
        activityNameLabel.configure(text=activityName)

def DoneActivity(e,activityTypeIndex, curActivityStatus,ActivityFrame,AIndex):
#     #print(type(AIndex))
#     #print(AIndex)
#     #print('AIndex in 723')
    Type=ActivityType[activityTypeIndex]
    actFrame=curActivityStatus[Type]

    ActObject={}

    if Type=='Match The Following':
        # print(Type)
        ActObject=saveMTF(actFrame)

    elif Type=='Fill In The Blank':
        # print(Type)
        ActObject=saveFITB(actFrame)

    elif Type=='MCQ':
        # print(Type)
        ActObject=saveMCQ(actFrame,curActivityStatus['MCQ Variable'])

    elif Type=='Long Answer':
        # print(Type)
        ActObject=saveLA(actFrame)

    AObject={}
    AObject['Type'] = Type
    AObject['Object'] = ActObject

    AIndex=str(AIndex)

    actModified=True
    obj=rootObject['Lessons'][LIndex]['Activities']
    if AIndex not in obj:
        AIndex = 'id'+AIndex
        # print('appended')
    else:
        if obj[AIndex]['Type']==Type:
            actModified=False
            AObject['Name']=obj[AIndex]['Name']
            # print('unmodified')
        #else:
        #     print('modified')
        # print(rootObject['Lessons'][LIndex]['Activities'][AIndex])

    rootObject['Lessons'][LIndex]['Activities'][AIndex] = AObject

    # print('ROOT AFTER DONE')
    # print(rootObject)
    #for i in rootObject['Lessons'][LIndex]['Activities']:
        # print(i)

    CollapseActivity(ActivityFrame,Type,AIndex,actModified)

    dbUpdate()

def setActivity(selectedIntVar,widget,val,curActivityStatus,AIndex):
#     #print(val.get())

    selectedVal=selectedIntVar.get()

#     #print (val)
    string=ActivityType[selectedVal]
    ActivityFrame=widget

    for i in range(1,len(ActivityFrame.winfo_children())):
        ActivityFrame.winfo_children()[i].pack_forget()

    if string=='MCQ':
        setMCQ(ActivityFrame,curActivityStatus,val,selectedVal)

    elif string=='Fill In The Blank':
        setFITB(ActivityFrame,curActivityStatus,val,selectedVal)
    elif string=='Match The Following':
        setMTF(ActivityFrame,curActivityStatus,val,selectedVal)

    elif string=='Long Answer':
        setLA(ActivityFrame, curActivityStatus, val, selectedVal)

    ActivityOpsFrame=Frame(ActivityFrame,bg='black')
    #delActivity=Label(ActivityOpsFrame,image=delImg)
    doneActivityBtn = Label(ActivityOpsFrame, image=TickImg30, cursor='hand2')
    # print('Aindex in 778: '+str(AIndex))
    doneActivityBtn.bind("<Button-1>",lambda e: DoneActivity(e,selectedIntVar.get(), curActivityStatus,ActivityFrame,AIndex))
    doneActivityBtn.pack(pady=10,padx=10)
    #delActivity.pack(pady=10)
    ActivityFrame.winfo_children()[-1].pack(expand=True)


def SetActivityTypes(event,activityFrame,aType=-1,AIndex=-1):
    v = IntVar()
    # print(aType)
    v.set(aType)  #
    ActivityTypeFrame=Frame(activityFrame,width=50,bg=ActivitiyTypeBG)

    Label(ActivityTypeFrame,
             text="Activity type:",
             padx = 20,bg='black',fg='white').pack(fill=X)

    curActivityStatus={}

    for val, type in enumerate(ActivityType):
        temp=Radiobutton(ActivityTypeFrame,
                    text=type,
                    indicatoron=0,
                    width=20,
                    padx=20,
                    variable=v,
                    bg=ActivitiyTypeBG,
                    command=lambda: setActivity(selectedIntVar=v,widget=activityFrame,val=aType,curActivityStatus=curActivityStatus, AIndex=AIndex),
                    value=val, selectcolor=SelectedActivityBG,fg='white')
        temp.pack(fill=X)

    ActivityTypeFrame.pack(anchor=W,padx=15,pady=15,side=LEFT)

    return v,curActivityStatus

def DelActivity(e,Aindex,ActivityOuter):
    # print(type(Aindex))
    if type(Aindex)==int:
        # print ('here')
        Aindex='id'+str(Aindex)
    elif 'id' not in Aindex:
        # print('here is the gandd')
        Aindex= 'id'+Aindex
    # print('AINDEX')
    # print(Aindex)
    del rootObject['Lessons'][LIndex]['Activities'][Aindex]
    ActivityOuter.destroy()
    dbUpdate()

def ANameChanged(e,Aindex, ow, cfont, ipadx):
    EntryToLabel(e, ow=ow, ipady=1, ipadx=ipadx, changeFont=cfont)
    # print('\n\n')
    # print(Aindex)
    # print(type(Aindex))
    if (type(Aindex)!=int):
        rootObject['Lessons'][LIndex]['Activities'][Aindex]['Name']=e.widget.get()
        dbUpdate()
    #     print ('Activity Name Updated to Database')
    # print(e.widget.get())
    # print('\n\n')

def addActivity(event,activitiesFrame,aType=-1,Aindex=-1):    #User added activity, aType = -1, load activity = activity object
    ActivityOuter=Frame(activitiesFrame,bg='black')
    ActivityCollapsedFrame=Frame(ActivityOuter,bg = ActivitiesBG)

    if Aindex not in rootObject['Lessons'][LIndex]['Activities']:
        rootObject['Lessons'][LIndex]['NextAID'] += 1
        # print('Incrementing')

    font='Arial 18 bold'
    ActivityNameFrame=addPairBox(ActivityCollapsedFrame,'','black','white',font,ipadx=0,ipady=5)
    c1=ActivityNameFrame.winfo_children()[0]
    c1.configure(width=50,height=2)
    c = ActivityNameFrame.winfo_children()[1]
    c.configure(width=50)

    #c.bind("<Leave>",lambda e: ANameChanged(e,Aindex))
    c.bind("<Return>", lambda e: ANameChanged(e,Aindex, ow=c1, cfont='Arial 18 bold', ipadx=0))
    c.bind("<Leave>", lambda e: ANameChanged(e,Aindex, ow=c1, cfont='Arial 18 bold', ipadx=0))
    ActivityNameFrame.pack(padx=20,side=LEFT)
    #ActivityLabel = Label(ActivityCollapsedFrame,text='Activity 1; Fill in the blanks',width=50,bg='black',fg='white',height=2, font='Arial 18 bold')
    #ActivityLabel.pack(padx=20, side=LEFT)

    ActivityDelBtn = Label(ActivityCollapsedFrame, image=delImg,bg='black',height=55,width=50,cursor='hand2')
    ActivityDelBtn.bind("<Button-1>",lambda e: DelActivity(e,Aindex,ActivityOuter))
    ActivityDelBtn.pack(padx=8, side=LEFT)

    ActivityEditBtn = Label(ActivityCollapsedFrame, image=edittImg30, bg='black',height=55,width=50,cursor='hand2')
    ActivityEditBtn.bind("<Button-1>",EditActivity)
    ActivityEditBtn.pack(padx=8,side= LEFT)

    Activity=Frame(ActivityOuter,bg='black')
    Activity.pack()
    ActivityOuter.pack(pady=25)
    intVar,curActivitySatus = SetActivityTypes(event,Activity,aType,Aindex)
    ActivityOuter.master.master.yview_moveto(2.0)
#     #print(Activity.winfo_children())

    return intVar,curActivitySatus

def SetActivities(Activity, LessonFrame):
    #ActivitiesFrame=Frame(LessonFrame,bg=ActivitiesBG)  # Transparent BG
    ActivitiesFrame=ScrollableFrame(LessonFrame,bg=ActivitiesBG,width='950')  # Transparent BG
    ActivitiesFrame.configure(bg=AddActivityBorder,bd=3)
    ActivitiesFrame.add_height=80
    Title = Label(LessonFrame,bg=ActivitiesBG,fg=ActivityTitle,font='Arial 26 bold',text=Activity['Name'].upper())   # Transparent BG
    Title.pack(pady=10)
    ActivitiesFrame.pack(expand=True, fill=BOTH)

    activitiesTable=ActivitiesFrame.scrollable_frame

    orderedActivities=[]
    for a in Activity['Activities']:
        j=0
        for i,key in enumerate(orderedActivities):
            if int(a[2:]) < int(key[2:]):
                break
            j+=1
        orderedActivities.insert(j, a)

    for i,a in enumerate(Activity['Activities']):
        a = orderedActivities[i]
        # print('i: '+str(i))
        # print(Activity['Activities'][a])
        obj=Activity['Activities'][a]
        intVar,curActivityStatus = addActivity(None,activitiesTable,obj['Object'],a)
        intVar.set(ActivityType.index(obj['Type']))
        setActivity(intVar,activitiesTable.winfo_children()[-1].winfo_children()[1],obj['Object'],curActivityStatus,a)

    return activitiesTable

def goToActivities(event,curLIndex):
    # print('curLindex: '+str(curLIndex))

    LessonObject=rootObject['Lessons'][curLIndex]
    global LIndex
    LIndex=curLIndex

    rootObject['Lessons'][curLIndex]=LessonObject
    # print(rootObject)

    HomeScreen.pack_forget()
    curActivity.pack(fill=BOTH,padx=0,pady=0)
    BackBtn.pack(anchor=W, padx = 10,pady=10)
    BackBtn.bind("<Enter>",BackKeyHoverIn)
    BackBtn.bind("<Leave>", BackKeyHoverOut)

    LessonFrame=Frame(curActivity,bg=ActivitiesBG)
    LessonFrame.pack(pady=20, padx=40)
    activitiesFrame=SetActivities(LessonObject, LessonFrame)
    activitiesFrame.configure(bg=ActivitiesBG,padx=10)

    BackBtn.bind("<Button-1>", lambda eff: BackToHomeScr(eff, LessonFrame=LessonFrame,LessonNActivities=event.widget))

    AddActivityBtnBorder = Frame(LessonFrame, bg=AddActivityBorder, borderwidth=3)
    AddActivityBtn = Button(AddActivityBtnBorder, bg=ActivitiesBG, bd=0, text='+ Add Activity', cursor='hand2',
                            fg=AddActivityText)
    AddActivityBtn.bind("<Button-1>",lambda eff: addActivity(eff, activitiesFrame=activitiesFrame,Aindex=LessonObject['NextAID']))
    AddActivityBtn.pack(ipadx=5)
    AddActivityBtnBorder.pack(pady=50)

    root.configure(bg=ActivitiesBG)

def LNameChanged(e,LIndex,ow, ipadx):
    EntryToLabel(e, ow=ow, ipadx=ipadx)
    rootObject['Lessons'][LIndex]['Name']=e.widget.get()
    # print('Lesson Name Updated')
    # print(rootObject)
    dbUpdate()

def addLesson(TableFrame,LIndex):

    # print('LIndex: '+LIndex)

    if 'Activities' not in rootObject['Lessons'][LIndex]:
        rootObject['Lessons'][LIndex]['Activities']={}
    # print(rootObject['Lessons'][LIndex]['Activities'])
    nActivities=len(rootObject['Lessons'][LIndex]['Activities'])

    LName=rootObject['Lessons'][LIndex]['Name']

    LessonRowFrame=Frame(TableFrame,background=HomeTableBG)
    LNameFrame= Frame(LessonRowFrame, background = HomeLNameBorder, borderwidth = 2)

    font=captionFont
    newLessonCaption='Type Lesson Name'
    if LName!=newLessonCaption:
        font=LNameFont
    LessonNameRead = Label(LNameFrame,text=LName,bg=HomeLNameBG,fg=HomeLNameText,width=20, font=font)
    LessonNameWrite = Entry(LNameFrame,bg=HomeLNameBG,fg=HomeLNameText,width=20,justify='center', font=LNameFont)

    LessonNameWrite.configure(insertbackground='white')

    LessonNameRead.bind("<Button-1>", lambda eff: LabelToEntry(eff, ow=LessonNameWrite,arr=[newLessonCaption]))
    LessonNameRead.bind("<Enter>", lambda eff: LNameHoverIn(eff,HomeLNameHoverBG))
    LessonNameRead.bind("<Leave>", lambda eff: LNameHoverOut(eff,HomeLNameBG))
    #LessonNameRead.bind("<Configure>", lambda e: LNameChanged(e, LIndex))

    LessonNameWrite.bind("<Leave>", lambda eff: LNameChanged(eff, LIndex, ow=LessonNameRead,ipadx=5))
    LessonNameWrite.bind("<Return>", lambda eff: LNameChanged(eff,LIndex, ow=LessonNameRead,ipadx=5))

    LessonActivities= Label(LessonRowFrame,text=str(nActivities)+" Activities",bg=HomeActivitiesBtnBG,fg='white',width=20,cursor = 'hand2')
    LessonActivities.bind("<Button-1>",lambda eff: goToActivities(eff,LIndex))

    LessonNameRead.pack(ipadx=5)
    LNameFrame.pack(side=LEFT,padx=10)

    LessonActivities.pack(side=LEFT,padx=10)

    DeleteBtn= Label(LessonRowFrame,image=delImg,cursor = 'hand2')
    DeleteBtn.bind("<Button-1>",lambda eff: RemoveLesson(eff, ow=LessonRowFrame,Tf=TableFrame,LIndex=LIndex))
    DeleteBtn.pack(side=LEFT,padx=40,pady=5)

    LessonRowFrame.pack(pady=5)

def addLessonBtn(event,bb,tf):
    bb.pack_forget()
    TablePack()

    LIndex='id'+str(rootObject['NextID'])
    rootObject['Lessons'][LIndex]={'Name':'Type Lesson Name', 'Activities':{},'NextAID':0}
    rootObject['NextID']+=1
    addLesson(tf,LIndex)
    bb.pack(pady=20)
    #for a in rootObject:
        # print('In root')
        # print(a)
        # print(rootObject[a])

def dbUpdate():
    try:
        db.update(rootObject)
    except:
        showNetworkDisconnected()

def showNetworkDisconnected():
    for a in root.winfo_children():
        a.pack_forget()
    nonetFrame = Frame(root,bd=100,relief='flat')
    nonetText = Label(nonetFrame, text='Connect to the Internet')
    nonetLabel = Label(nonetFrame, image=noNetwork100)
    nonetText.pack(fill=X, anchor=W, side=LEFT)
    nonetLabel.pack(fill=X, anchor=E)
    nonetFrame.pack()





config = {"apiKey": "AIzaSyBoR3NQYVhhP2wcvSsbaGEoNypEcQciSB8",
    "authDomain": "activitiesdata.firebaseapp.com",
    "databaseURL": "https://activitiesdata.firebaseio.com",
    "projectId": "activitiesdata",
    "storageBucket": "activitiesdata.appspot.com",
    "messagingSenderId": "836115710939",
    "appId": "1:836115710939:web:b167230878266a74dd5dfe",
    "measurementId": "G-KF6TL6QSQJ"
}

rootObject={}

db={}

try:
    firebase=pyrebase.initialize_app(config)

    db = firebase.database()

    rootObject=db.get().val()
    # print('A')
    # print(rootObject)
    # print(rootObject.values())

    HomeScreen.pack(fill=BOTH, padx=0, pady=0)

    TablePack()

    ButtonBorder = Frame(HomeScreen, background=HomeAddLessonBorder, borderwidth=3)
    AddLesson = Button(ButtonBorder, text="+ Add Lesson", cursor='hand2')
    AddLesson.bind("<Button-1>", lambda eff: addLessonBtn(eff, bb=ButtonBorder, tf=TableFrame))
    AddLesson.configure(bg=HomeAddLessonBG, fg=HomeAddLessonText, bd=0)

    ButtonBorder.pack(pady=20)
    AddLesson.pack()



except:
    showNetworkDisconnected()


LIndex='id0'

if 'Lessons' in rootObject:
    orderedLessons=[]
    for a in rootObject['Lessons']:
        j=0
        for i,key in enumerate(orderedLessons):
            if int(a[2:]) < int(key[2:]):
                break
            j+=1
        orderedLessons.insert(j, a)

    for i,id in enumerate(rootObject["Lessons"]):
        id = orderedLessons[i]
        LIndex=id
        addLesson(TableFrame,LIndex)
else:
    rootObject['Lessons']={}


root.mainloop()