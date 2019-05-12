package com.zhiming.li;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.Objects;
import java.util.Set;

//识别活前缀的DFA类
public class LR1DFA {

    //下标
    private int index;
    //表示一个规范族
    private ArrayList<LR1> lr1s;
    //存储连接边
    private HashMap<String,LR1DFA> adjacentSide;
    //是否经过Go函数处理
    private Boolean afterGo;

    public int getIndex() {
        return index;
    }

    public void setIndex(int index) {
        this.index = index;
    }

    public ArrayList<LR1> getLr1s() {
        return lr1s;
    }

    public void setLr1s(ArrayList<LR1> lr1s) {
        this.lr1s = lr1s;
    }

    public HashMap<String, LR1DFA> getAdjacentSide() {
        return adjacentSide;
    }

    public void setAdjacentSide(HashMap<String, LR1DFA> adjacentSide) {
        this.adjacentSide = adjacentSide;
    }

    public Boolean getAfterGo() {
        return afterGo;
    }

    public void setAfterGo(Boolean afterGo) {
        this.afterGo = afterGo;
    }

    public LR1DFA(int index, ArrayList<LR1> lr1s, HashMap<String, LR1DFA> adjacentSide) {
        this.index = index;
        this.lr1s = lr1s;
        this.adjacentSide = adjacentSide;
        this.afterGo = false;
    }

    @Override
    public int hashCode() {
        return Objects.hash(index,lr1s,adjacentSide);
    }

    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if (obj == null || getClass() != obj.getClass()) return false;
        LR1DFA that = (LR1DFA) obj;
        return index == that.index &&
                Objects.equals(lr1s, that.lr1s) &&
                Objects.equals(adjacentSide, that.adjacentSide);
    }

    @Override
    public String toString() {
        System.out.println("I" + index + ":");
        System.out.println("项目族：");
        for (LR1 lr1 : lr1s) {
            System.out.println("\t" + lr1.toString());
        }
        System.out.println("邻边：");
        Set<String> keySet = adjacentSide.keySet();
        for (String key : keySet) {
            System.out.println("\t" + key + "->" + adjacentSide.get(key).getIndex());
        }
        System.out.println("--------------------------------------------");
        return "";
    }
}
