#pragma once

class MemoryRegister {
public:
    void clear() {
        m_memory = 0.0;
        m_hasMemory = false;
    }

    void store(double val) {
        m_memory = val;
        m_hasMemory = true;
    }

    double recall() const {
        return m_memory;
    }

    void add(double val) {
        m_memory += val;
        m_hasMemory = true;
    }

    void subtract(double val) {
        m_memory -= val;
        m_hasMemory = true;
    }

    bool hasMemory() const {
        return m_hasMemory;
    }

private:
    double m_memory = 0.0;
    bool m_hasMemory = false;
};

