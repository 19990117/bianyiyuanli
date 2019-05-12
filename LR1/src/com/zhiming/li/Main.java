package com.zhiming.li;

import java.io.*;
import java.util.ArrayList;
import java.util.HashMap;
import java.util.Scanner;
import java.util.StringTokenizer;

/*
E->S
S->A
A->BA
A->ε
B->aB
B->b
end

E->S
S->BB
B->aB
B->b
end
*/
public class Main {
    public static void main(String[] args){
        //输入并求first集
        ArrayList<String[]> input = getInput();
        ArrayList<String> allNonTerminals = getAllNonTerminals(input);
        ArrayList<String> nonTerminalsNullable = getAllNonTerminalsNullable(input);
        HashMap<String, ArrayList<String>> first = getFirst(allNonTerminals, nonTerminalsNullable, input);

        //将文法转存到类中
        ArrayList<ProduceFormula> produceFormulas = new ArrayList<>();
        for (String[] strings : input) {
            ProduceFormula productionFormula = new ProduceFormula(strings[0], strings[1]);
            produceFormulas.add(productionFormula);
        }

        String begin = produceFormulas.get(0).getLeft();
        ArrayList<LR1> lr1s = new ArrayList<>();//项目族
        ArrayList<LR1DFA> DFA = new ArrayList<>();//DFA
        //手动构造I0，以后的循环产生
        //先把拓广文法加入I0
        LR1 lr1 = new LR1(produceFormulas.get(0),"",produceFormulas.get(0).getRight(),"#",allNonTerminals,begin);
        lr1s.add(lr1);
        LR1DFA I0 = new LR1DFA(0,lr1s,new HashMap<>());
        DFA.add(I0);
        closure(I0,produceFormulas,allNonTerminals,first,begin);//求I0的closure集合

        ArrayList<LR1DFA> cloneDFA = new ArrayList<>(DFA);
        while (!allAfterGo(DFA)){
            for (LR1DFA I:DFA)
                if (!I.getAfterGo()){
                    go(I,cloneDFA,allNonTerminals,begin,produceFormulas,first);
                }
             DFA.clear();
            DFA.addAll(cloneDFA);
        }
        System.out.println(DFA);
    }

    //判断项目族是否均进行了closure
    private static Boolean allAfterClosure(LR1DFA I){
        ArrayList<LR1> lr1s = I.getLr1s();
        for(LR1 p:lr1s){
            //如果产生式圆点以后还有字符
            if (p.getWait()){
                //判断这个产生式是否是处理过的
                if (!p.getAfterClosure()){
                    return false;
                }
            }
        }
        return true;
    }

    //通过产生式的左部来获取产生式
    private static ArrayList<ProduceFormula> getProductionFormulaByLeft(ArrayList<ProduceFormula> produceFormulas, String left) {
        ArrayList<ProduceFormula> res = new ArrayList<>();
        for (ProduceFormula p : produceFormulas) {
            if (p.getLeft().equals(left)) {
                res.add(p);
            }
        }
        return res;
    }

    //求closure产生的项目
    private static ArrayList<LR1> getLR1FromClosure(LR1 lr1,ArrayList<ProduceFormula> produceFormulas, ArrayList<String> allNonTerminals, HashMap<String, ArrayList<String>> first, String begin){
        ArrayList<LR1> res = new ArrayList<>();
        //点后面是非终结符，还需要求出他的产生式
        if (lr1.getAfterPoint().length() >= 1){
            String left = lr1.getAfterPoint().substring(0,1);
            //当前项目的点之后的部分
            String currentPF = lr1.getAfterPoint();
            //当前项目逗号后的部分
            String currentAFC = lr1.getForWant();
            //遇到非终结符就将它的产生式加入
            ArrayList<ProduceFormula> produceFormulas1 = getProductionFormulaByLeft(produceFormulas,left);
            //给新加的产生式加点以及加展望
            for (ProduceFormula p:produceFormulas1){
                String resOfBeforePoint = "";
                String resOfAfterPoint = p.getRight();
                if (p.getRight().equals("ε")){//产生式推出为空时，只有一个点
                    resOfAfterPoint = "";
                }
                String resOfForWant = "";
                //当当前圆点之后只有一位时，展望信息为#，添加至原展望符号之内 S->.S
                if (currentPF.length() <= 1){
                    resOfForWant = "#";
                    char[] chars = currentAFC.toCharArray();
                    for (char have : chars){//如果原展望式中有#,就不用管了
                        if (!resOfForWant.contains(String.valueOf(have))){
                            resOfForWant += String.valueOf(have);
                        }
                    }
                    //如果展望式是继承下来的话，就不用管#了
                    if (resOfForWant.length() > 1)
                        resOfForWant = resOfForWant.replace("#","");
                }else {
                    String s = currentPF.substring(currentPF.indexOf(".") + 2);
                    if (s.equals("")){
                        resOfForWant = "#";
                        char[] chars = currentAFC.toCharArray();
                        for (char have : chars)
                            if (!resOfForWant.contains(String.valueOf(have))){
                                resOfForWant += String.valueOf(have);
                            }
                        if (resOfForWant.length() > 1)
                            resOfForWant = resOfForWant.replace("#","");
                    }else {
                        //求first集加入展望式
                        ArrayList<String> strings = getStringFirst(allNonTerminals,first,currentPF.toCharArray());
                        for (String resString : strings)
                            resOfForWant += resString;
                        char[] chars = currentAFC.toCharArray();
                        for (char have : chars)
                            if (!resOfForWant.contains(String.valueOf(have)))
                                resOfForWant += String.valueOf(have);
                        if (resOfForWant.length() > 1)
                            resOfForWant = resOfForWant.replace("#","");
                    }
                }
                LR1 lr11 = new LR1(p,resOfBeforePoint,resOfAfterPoint,resOfForWant,allNonTerminals,begin);
                res.add(lr11);
            }
        }else {
            return res;
        }
        return res;
    }

    //求closure产生的项目族
    private static void closure(LR1DFA I,ArrayList<ProduceFormula> produceFormulas,ArrayList<String> allNonTerminals,HashMap<String,ArrayList<String>> first,String begin){
        ArrayList<LR1> lr1s = I.getLr1s();
        ArrayList<LR1> clone = new ArrayList<>(lr1s);
        //遍历项目族里的项目
        while (!allAfterClosure(I)){
            for (LR1 lr1:lr1s){
                //closure处理的是待约项目
                if (lr1.getWait() && !lr1.getAfterClosure()){
                    ArrayList<LR1> lr1s1 = getLR1FromClosure(lr1,produceFormulas,allNonTerminals,first,begin);
                    lr1.setAfterClosure(true);
                    clone.addAll(lr1s1);
                }
            }
            lr1s.clear();
            lr1s.addAll(clone);
        }
    }

    //判断项目族中项目是否全都经过go处理
    private static Boolean allAfterGo(ArrayList<LR1DFA> DFA) {
        for (LR1DFA I : DFA)
            if (!I.getAfterGo()) {
                return false;
            }
        return true;
    }

    //go函数实现过后的新项目
    private static LR1 makeLR1ForGo(LR1 lr1, ArrayList<String> allNonTerminals, String begin) {
        String beforePoint = lr1.getBeforePoint();
        String afterPoint = lr1.getAfterPoint();
        String newBeforePoint = beforePoint + afterPoint.substring(0, 1);
        String newAfterPoint = afterPoint.substring(1);
        return new LR1(lr1.getProduceFormula(), newBeforePoint, newAfterPoint, lr1.getForWant(), allNonTerminals, begin);
    }

    //判断项目是否被包含
    private static int notNew(ArrayList<LR1DFA> DFA, LR1 lr1) {
        for (LR1DFA dfa : DFA) {
            for (LR1 l : dfa.getLr1s()) {
                if (l.equals(lr1)) {
                    return dfa.getIndex();
                }
            }
        }
        return -1;
    }

    //通过index找I
    private static LR1DFA getLR1DFAByIndex(int index, ArrayList<LR1DFA> DFA) {
        for (LR1DFA I : DFA) {
            if (I.getIndex() == index) {
                return I;
            }
        }
        return null;
    }

    //go函数
    private static void go(LR1DFA I,ArrayList<LR1DFA> DFA, ArrayList<String> allNonTerminals, String begin, ArrayList<ProduceFormula> productionFormulas, HashMap<String, ArrayList<String>> first){
        for (LR1 lr1:I.getLr1s()){
            if (lr1.getReduce()){
                continue;
            }
            String afterPoint = lr1.getAfterPoint();
            if (afterPoint.length() != 0){
                String pointFirstChar = afterPoint.substring(0,1);
                //go产生的新项目
                LR1 newLr1 = makeLR1ForGo(lr1,allNonTerminals,begin);
                int res = notNew(DFA,newLr1);
                if (res == -1){//判断是否是新的项目族
                    int index =  DFA.size();
                    ArrayList<LR1> newLR1DFA = new ArrayList<>();
                    newLR1DFA.add(newLr1);
                    LR1DFA newI = new LR1DFA(index,newLR1DFA,new HashMap<>());
                    //对新I调用closure
                    closure(newI,productionFormulas,allNonTerminals,first,begin);
                    DFA.add(newI);
                    //将边加入到上一个I中
                    I.getAdjacentSide().put(pointFirstChar,getLR1DFAByIndex(index,DFA));
                }else {
                    I.getAdjacentSide().put(pointFirstChar,getLR1DFAByIndex(res,DFA));
                }
            }else break;
        }
        I.setAfterGo(true);
    }

    //分析输入
    private static ArrayList<String[]> getInput() {
        ArrayList<String[]> input = new ArrayList<>();
        Scanner sc = new Scanner(System.in);
        System.out.println("请分行输入一个完整文法:(end结束)");
        String sline;
        sline = sc.nextLine();
        while (!sline.startsWith("end")) {
            StringBuilder buffer = new StringBuilder(sline);
            int l = buffer.indexOf(" ");
            //去除空格
            while (l >= 0) {
                buffer.delete(l, l + 1);
                l = buffer.indexOf(" ");
            }
            sline = buffer.toString();
            //s存储左推导符（既非终结符）
            String[] s = sline.split("->");
            if (s.length == 1) {
                System.out.println("文法有误");
                System.exit(0);
            }
            StringTokenizer fx = new StringTokenizer(s[1], "|︱");
            //如果产生式的右部出现了 | 则按多条产生式进行存储
            while (fx.hasMoreTokens()) {
                String[] productionFormula = new String[2];
                productionFormula[0] = s[0].trim();//0的位置放非终结符
                productionFormula[1] = fx.nextToken().trim();//1的位置放导出的产生式
                input.add(productionFormula);
            }
            sline = sc.nextLine();
        }
        return input;
    }

    //求非终结符集合
    private static ArrayList<String> getAllNonTerminals(ArrayList<String[]> input) {
        ArrayList<String> nonTerminals = new ArrayList<>();
        for (String[] strings : input) {
            String nonTerminal = strings[0];
            if (!nonTerminals.contains(nonTerminal)) {
                nonTerminals.add(nonTerminal);
            }
        }
        return nonTerminals;
    }

    //判断哪个非终结符能推出ε
    private static ArrayList<String> getAllNonTerminalsNullable(ArrayList<String[]> input) {
        ArrayList<String> nonTerminalsNullable = new ArrayList<>();
        for (String[] strings : input) {
            if (strings[1].equals("ε")) {
                nonTerminalsNullable.add(strings[0]);
            }
        }
        return nonTerminalsNullable;
    }

    /**
     * 对象深度复制(对象必须是实现了Serializable接口)
     *
     * @param obj 被复制的对象
     * @return T
     * @author Muscleape
     */
    @SuppressWarnings("unchecked")
    private static <T extends Serializable> T clone(T obj) {
        T clonedObj = null;
        try {
            ByteArrayOutputStream baos = new ByteArrayOutputStream();
            ObjectOutputStream oos = new ObjectOutputStream(baos);
            oos.writeObject(obj);
            oos.close();

            ByteArrayInputStream bais = new ByteArrayInputStream(baos.toByteArray());
            ObjectInputStream ois = new ObjectInputStream(bais);
            clonedObj = (T) ois.readObject();
            ois.close();
        } catch (Exception e) {
            e.printStackTrace();
        }
        return clonedObj;
    }

    /**
     * 求解串的FIRST集
     *
     * @param allNonTerminals 所有的非终结符
     * @param first           first集
     * @param chars           待求串
     * @return 串的first集合
     */
    private static ArrayList<String> getStringFirst
    (ArrayList<String> allNonTerminals, HashMap<String, ArrayList<String>> first,
     char[] chars) {
        ArrayList<String> stringFirst = new ArrayList<>();
        for (int i = 0; i < chars.length; i++) {
            //1.向stringFirst中加入First(char[0])的非ε元素
            if (i == 0) {
                ArrayList<String> currentFirst = getCharFirst(allNonTerminals, first, chars[i]);
                for (String s : currentFirst) {
                    if (!s.equals("ε") && !stringFirst.contains(s)) {
                        stringFirst.add(s);
                    }
                }
            }
            //2如果current为最后一个字符，且first(current)包含ε，则将ε加入stringFirst
            if (i == chars.length - 1) {
                ArrayList<String> currentFirst = getCharFirst(allNonTerminals, first, chars[i]);
                if (currentFirst.contains("ε")) {
                    if (!stringFirst.contains("ε")) {
                        stringFirst.add("ε");
                    }
                }
            } else {//current不是最后一个字符
                ArrayList<String> currentFirst = getCharFirst(allNonTerminals, first, chars[i]);
                ArrayList<String> nextFirst = getCharFirst(allNonTerminals, first, chars[i + 1]);

                if (currentFirst.contains("ε")) {//3.1如果FIRST(current)包含ε，向stringFirst中加入First(char[i+1])的非ε元素
                    for (String s : nextFirst) {
                        if (!s.equals("ε") && !stringFirst.contains(s)) {
                            stringFirst.add(s);
                        }
                    }
                } else { //3.2如果FIRST(current)不包含ε,向stringFirst中加入First(current)的非ε元素并break
                    for (String s : currentFirst) {
                        if (!s.equals("ε") && !stringFirst.contains(s)) {
                            stringFirst.add(s);
                        }
                    }
                    break;
                }
            }
        }
        return stringFirst;
    }

    /**
     * 求解一个符号的first集
     *
     * @param allNonTerminals 所有的非终结符
     * @param first           first集
     * @param c               等待求解的字符
     * @return ArrayList
     */
    private static ArrayList<String> getCharFirst
    (ArrayList<String> allNonTerminals, HashMap<String, ArrayList<String>> first, char c) {
        ArrayList<String> charFirst = new ArrayList<>();
        String theChar = String.valueOf(c);
        if (theChar.equals("ε")) {
            charFirst.add("ε");
        }
        //如果字符为终结符
        if (!allNonTerminals.contains(theChar)) {
            charFirst.add(theChar);
        } else {//如果为非终结符
            ArrayList<String> theFirst = first.get(theChar);
            for (String s : theFirst) {
                if (!charFirst.contains(s)) {
                    charFirst.add(s);
                }
            }
        }
        return charFirst;
    }

    /**
     * 求解First集合
     *
     * @param allNonTerminals      文法中所有的非终结符
     * @param nonTerminalsNullable 文法中可直接推出ε的非终结符
     * @param input                输入的文法
     * @return first集
     */
    private static HashMap<String, ArrayList<String>> getFirst(ArrayList<String> allNonTerminals, ArrayList<String> nonTerminalsNullable, ArrayList<String[]> input) {
        HashMap<String, ArrayList<String>> first = new HashMap<>();
        //先将非终结符加入到first中
        for (String s : allNonTerminals) {
            first.put(s, new ArrayList<>());
        }
        //计算first集的过程
        //1.1若右边第一个符号 ε  ，则直接将其加入 First（X）
        for (String s : nonTerminalsNullable) {
            ArrayList<String> strings = first.get(s);
            if (strings == null) {
                strings = new ArrayList<>();
            }
            strings.add("ε");
            first.put(s, strings);
        }
        //1.2若右边第一个符号是终结符,即小写字母 ，则直接将其加入 First（X）
        for (String[] strings1 : input) {
            String productionFormula = strings1[1];
            if (!Character.isUpperCase(productionFormula.charAt(0)) && !productionFormula.equals("ε")) {
                ArrayList<String> strings = first.get(strings1[0]);
                if (strings == null) {
                    strings = new ArrayList<>();
                }
                strings.add(String.valueOf(productionFormula.charAt(0)));
                first.put(strings1[0], strings);
            }
        }
        HashMap<String, ArrayList<String>> beforLoop = clone(first);
        HashMap<String, ArrayList<String>> afterLoop = new HashMap<>();
        while (!beforLoop.equals(afterLoop)) {
            beforLoop = clone(first);
            //2.若右边第一个符号是非终结符，则将其 First 集的的非 ε  元素加入 First（X）
            for (String[] strings1 : input) {
                String productionFormula = strings1[1];
                if (Character.isUpperCase(productionFormula.charAt(0))) {
                    //firstNonTerminal 产生式右部的第一个非终结符
                    String firstNonTerminal = String.valueOf(productionFormula.charAt(0));
                    //currentNonTerminal 产生式左部的非终结符
                    String currentNonTerminal = strings1[0];
                    //currentFirst 当前产生式左部的first集
                    ArrayList<String> currentFirst = first.get(currentNonTerminal);
                    //firstNonTerminalFirst 当前产生式右部的第一个非终结符的first集合
                    ArrayList<String> firstNonTerminalFirst = first.get(firstNonTerminal);
                    for (String s : firstNonTerminalFirst) {
                        if (!s.equals("ε") && !currentFirst.contains(s)) {
                            currentFirst.add(s);
                        }
                    }
                    first.put(currentNonTerminal, currentFirst);
                }
            }
            //若右边第一个符号是非终结符而且紧随其后的是很多个非终结符，这个时候就要注意是否有 ε
            //3.1 第 i 个非终结符的 First 集有 ε  ，则可将第 i+1 个非终结符去除 ε  的 First 集加入 First（X）
            //3.2 若所有的非终结符都能够推导出 ε ，则将  ε  也加入到 First（X）
            for (String[] strings : input) {
                String productionFormula = strings[1];
                ArrayList<String> currentFirst = first.get(strings[0]);
                //产生式右部第一个符号为非终结符
                if (Character.isUpperCase(productionFormula.charAt(0))) {
                    //将产生式转为字符数组
                    char[] chars = productionFormula.toCharArray();

                    //遍历字符数组
                    for (int j = 0; j < chars.length; j++) {
                        //如果碰到终结符
                        if (!Character.isUpperCase(chars[j])) {
                            break;
                        }
                        //第j个字符的first集 jCharFirst
                        ArrayList<String> jCharFirst = first.get(String.valueOf(chars[j]));
                        if (jCharFirst != null && jCharFirst.contains("ε")) {
                            if (j + 1 < chars.length) {
                                if (Character.isUpperCase(chars[j + 1])) {
                                    ArrayList<String> jNextFirst = first.get(String.valueOf(chars[j + 1]));
                                    for (String s : jNextFirst) {
                                        if (!s.equals("ε") && !currentFirst.contains(s)) {
                                            currentFirst.add(s);
                                        }
                                    }
                                } else {
                                    if (!String.valueOf(chars[j + 1]).equals("ε") && !currentFirst.contains(String.valueOf(chars[j + 1]))) {
                                        currentFirst.add(String.valueOf(chars[j + 1]));
                                    }
                                }
                                first.put(strings[0], currentFirst);
                            } else {
                                if (!currentFirst.contains("ε")) {
                                    currentFirst.add("ε");
                                    first.put(strings[0], currentFirst);
                                }
                            }
                        } else {
                            break;
                        }
                    }
                }
            }
            afterLoop = clone(first);
        }
        return first;
    }
}
