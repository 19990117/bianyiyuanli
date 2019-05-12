package com.zhiming.li;

import java.util.ArrayList;
import java.util.Objects;

//LR1项目的类
public class LR1 {

    //产生式
    private ProduceFormula produceFormula;
    //产生式右部圆点之前
    private String beforePoint;
    //产生式右部圆点之后
    private String afterPoint;
    //展望信息
    private String forWant;

    //根据圆点位置所确认的项目状态
    private Boolean isShift;
    private Boolean isWait;
    private Boolean isReduce;
    private Boolean isAccept;
    //是否加入了closure函数过后的产生式
    private Boolean afterClosure;

    public ProduceFormula getProduceFormula() {
        return produceFormula;
    }

    public void setProduceFormula(ProduceFormula produceFormula) {
        this.produceFormula = produceFormula;
    }

    public String getBeforePoint() {
        return beforePoint;
    }

    public void setBeforePoint(String beforePoint) {
        this.beforePoint = beforePoint;
    }

    public String getAfterPoint() {
        return afterPoint;
    }

    public void setAfterPoint(String afterPoint) {
        this.afterPoint = afterPoint;
    }

    public String getForWant() {
        return forWant;
    }

    public void setForWant(String forWant) {
        this.forWant = forWant;
    }

    public Boolean getShift() {
        return isShift;
    }

    public void setShift(Boolean shift) {
        isShift = shift;
    }

    public Boolean getWait() {
        return isWait;
    }

    public void setWait(Boolean wait) {
        isWait = wait;
    }

    public Boolean getReduce() {
        return isReduce;
    }

    public void setReduce(Boolean reduce) {
        isReduce = reduce;
    }

    public Boolean getAccept() {
        return isAccept;
    }

    public void setAccept(Boolean accept) {
        isAccept = accept;
    }

    public Boolean getAfterClosure() {
        return afterClosure;
    }

    public void setAfterClosure(Boolean afterClosure) {
        this.afterClosure = afterClosure;
    }

    public LR1() {
    }

    /*
    *初始化LR1项目时，根据点后面的字符串来判断项目状态
    *allNonTerminals非终结符集
    *begin文法开始符号
    * */
    public LR1(ProduceFormula produceFormula, String beforePoint, String afterPoint, String forWant, ArrayList<String> allNonTerminals, String begin) {
        this.produceFormula = produceFormula;
        this.beforePoint = beforePoint;
        this.afterPoint = afterPoint;
        this.forWant = forWant;
        this.isShift = false;
        this.isReduce = false;
        this.isWait = false;
        this.isAccept = false;
        this.afterClosure = false;
        if (afterPoint.length() == 0) {
            //判断是否是接受项目
            if (produceFormula.getLeft().equals(begin)) {
                isAccept = true;
            }
            this.isReduce = true;//规约项目
        } else if (allNonTerminals.contains(afterPoint.substring(0, 1))) {
            //非终结符--待约项目
            this.isWait = true;
        } else {
            //终结符--移进项目
            this.isShift = true;
        }
    }

    @Override
    public String toString() {
        return produceFormula.getLeft() + "->" + beforePoint + "." + afterPoint + "," + forWant;
    }

    //判断closure时是否会重复
    @Override
    public boolean equals(Object obj) {
        if (this == obj) return true;
        if(obj == null || getClass() != obj.getClass()) return false;
        LR1 it = (LR1)obj;
        return Objects.equals(produceFormula, it.produceFormula) &&
                Objects.equals(beforePoint, it.beforePoint) &&
                Objects.equals(afterPoint, it.afterPoint) &&
                Objects.equals(forWant, it.forWant) &&
                Objects.equals(isShift, it.isShift) &&
                Objects.equals(isReduce, it.isReduce) &&
                Objects.equals(isAccept, it.isAccept) &&
                Objects.equals(isWait, it.isWait);
    }

    @Override
    public int hashCode() {
        return Objects.hash(produceFormula,beforePoint,afterPoint,forWant,isShift,isReduce,isAccept,isWait);
    }
}
