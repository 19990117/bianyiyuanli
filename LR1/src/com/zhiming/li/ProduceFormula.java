package com.zhiming.li;

//产生式类
public class ProduceFormula {
    private String left;
    private String right;

    public ProduceFormula(String left, String right) {
        this.left = left;
        this.right = right;
    }

    public ProduceFormula() {
    }

    public String getLeft() {
        return left;
    }

    public void setLeft(String left) {
        this.left = left;
    }

    public String getRight() {
        return right;
    }

    public void setRight(String right) {
        this.right = right;
    }

    @Override
    public String toString() {
        return left + "->" + right;
    }
}
