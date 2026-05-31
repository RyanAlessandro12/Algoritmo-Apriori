#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <sstream>
#include <limits>

using namespace std;

// Conta o suporte de cada candidato nas transações
map<vector<string>, int> contarSoporte(
    const set<vector<string>>& candidatos,
    const map<string, vector<string>>& transacciones)
{
    map<vector<string>, int> conteo;

    for (const auto& [tid, items] : transacciones) {
        set<string> items_set(items.begin(), items.end());

        for (const auto& candidato : candidatos) {
            bool subset = true;
            for (const auto& item : candidato) {
                if (items_set.find(item) == items_set.end()) {
                    subset = false;
                    break;
                }
            }
            if (subset) {
                conteo[candidato]++;
            }
        }
    }

    return conteo;
}

// Imprime um conjunto de itens
void printConjunto(const vector<string>& conjunto) {
    cout << "(";
    for (size_t i = 0; i < conjunto.size(); ++i) {
        cout << conjunto[i];
        if (i + 1 < conjunto.size()) cout << ", ";
    }
    cout << ")";
}

string trim(const string& s) {
    const string whitespace = " \t\r\n";
    size_t start = s.find_first_not_of(whitespace);
    if (start == string::npos) return "";
    size_t end = s.find_last_not_of(whitespace);
    return s.substr(start, end - start + 1);
}

bool parseTransactionLine(const string& line, string& tid, vector<string>& items) {
    string trimmed = trim(line);
    if (trimmed.empty()) return false;

    vector<string> tokens;
    string token;
    stringstream ss(trimmed);
    while (getline(ss, token, ',')) {
        token = trim(token);
        if (!token.empty()) {
            tokens.push_back(token);
        }
    }

    if (tokens.size() < 2) {
        return false;
    }

    tid = tokens[0];
    items.assign(tokens.begin() + 1, tokens.end());
    return true;
}

int main() {
    map<string, vector<string>> transacciones;

    cout << "Ingrese transacciones en formato TID,I1,I2,..." << endl;
    cout << "Por ejemplo: T100,I1,I2,I5" << endl;
    cout << "Ingrese una transacción por línea. Deje una línea vacía para terminar." << endl;

    string line;
    while (true) {
        cout << "> ";
        if (!getline(cin, line) || trim(line).empty()) {
            break;
        }

        string tid;
        vector<string> items;
        if (!parseTransactionLine(line, tid, items)) {
            cout << "Formato inválido. Use: T100,I1,I2,..." << endl;
            continue;
        }

        transacciones[tid] = items;
    }

    if (transacciones.empty()) {
        cout << "No se ingresaron transacciones. Se usarán datos de ejemplo." << endl;
        transacciones = {
            {"T100", {"I1", "I2", "I5"}},
            {"T200", {"I2", "I4"}},
            {"T300", {"I2", "I3"}},
            {"T400", {"I1", "I2", "I4"}},
            {"T500", {"I1", "I3"}},
            {"T600", {"I2", "I3"}},
            {"T700", {"I1", "I3"}},
            {"T800", {"I1", "I2", "I3", "I5"}},
            {"T900", {"I1", "I2", "I3"}}
        };
    }

    int SupMin;
    while (true) {
        cout << "Ingrese soporte mínimo (entero >= 1): ";
        if (cin >> SupMin && SupMin >= 1) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            break;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Soporte inválido. Ingrese un número entero mayor o igual a 1." << endl;
    }

    // --- F1: Conjuntos frequentes de tamanho 1 ---
    map<vector<string>, int> conteo_items;

    for (const auto& [tid, items] : transacciones) {
        for (const auto& item : items) {
            conteo_items[{item}]++;
        }
    }

    map<vector<string>, int> F1;
    for (const auto& [item, soporte] : conteo_items) {
        if (soporte >= SupMin) {
            F1[item] = soporte;
        }
    }

    cout << "F1 (Conjuntos frecuentes de tamaño 1)" << endl;
    for (const auto& [item, soporte] : F1) {
        printConjunto(item);
        cout << " -> soporte = " << soporte << endl;
    }

    // --- APRIORI ---
    map<vector<string>, int> Fk = F1;
    int k = 1;

    map<vector<string>, int> todos_frecuentes = F1;

    while (!Fk.empty()) {
        k++;

        // Gerar candidatos Ck
        vector<vector<string>> items_frecuentes;
        for (const auto& [item, _] : Fk) {
            items_frecuentes.push_back(item);
        }

        set<vector<string>> candidatos;

        for (size_t i = 0; i < items_frecuentes.size(); ++i) {
            for (size_t j = i + 1; j < items_frecuentes.size(); ++j) {
                set<string> union_set(items_frecuentes[i].begin(), items_frecuentes[i].end());
                union_set.insert(items_frecuentes[j].begin(), items_frecuentes[j].end());

                if ((int)union_set.size() == k) {
                    vector<string> union_vec(union_set.begin(), union_set.end());
                    sort(union_vec.begin(), union_vec.end());
                    candidatos.insert(union_vec);
                }
            }
        }

        cout << "\nC" << k << " (Candidatos)" << endl;
        for (const auto& c : candidatos) {
            printConjunto(c);
            cout << endl;
        }

        // Contar suporte
        map<vector<string>, int> conteo = contarSoporte(candidatos, transacciones);

        // Gerar Fk
        Fk.clear();
        for (const auto& [candidato, soporte] : conteo) {
            if (soporte >= SupMin) {
                Fk[candidato] = soporte;
            }
        }

        cout << "\nF" << k << " (Frecuentes)" << endl;
        for (const auto& [item, soporte] : Fk) {
            printConjunto(item);
            cout << " -> soporte = " << soporte << endl;
        }

        todos_frecuentes.insert(Fk.begin(), Fk.end());
    }

    cout << "\nTODOS LOS CONJUNTOS FRECUENTES" << endl;
    for (const auto& [conjunto, soporte] : todos_frecuentes) {
        printConjunto(conjunto);
        cout << " -> soporte = " << soporte << endl;
    }

    return 0;
}