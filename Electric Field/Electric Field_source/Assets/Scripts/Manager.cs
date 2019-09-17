using System;
using System.Collections;
using System.Collections.Generic;
using UnityEngine;

public class Manager : MonoBehaviour
{
    static int cntx = 24, cnty = 24;
    float ck = 100.0f, siz = 5.0f;
    public GameObject obj, scroll, ball, arrow, parrow;
    public Material Bmat, Rmat;
    List<GameObject> ec = new List<GameObject>();
    List<GameObject> B = new List<GameObject>();
    GameObject[, ] A = new GameObject[cntx, cnty];
    List<int> X = new List<int>();
    List<int> Y = new List<int>();
    List<int> Q = new List<int>();
    void Start()
    {
        for (int i = 0; i < cntx; i++)
            for (int j = 0; j < cnty; j++) {
                A[i, j] = Instantiate(arrow, parrow.transform);
                A[i, j].transform.position =
                    new Vector3(1024.0f / cntx * (i - cntx / 2.0f), 768.0f / cnty * (j - cnty / 2.0f), 0.0f);
                A[i, j].transform.GetChild(0).localScale = new Vector3(0.0f, 0.0f, 0.0f);
                A[i, j].transform.GetChild(1).localScale = new Vector3(0.0f, 0.0f, 0.0f);
            }
    }
    void cal()
    {
        for(int i = 0; i < cntx; i++)
        {
            for(int j = 0; j < cnty; j++)
            {
                Vector3 ans = new Vector3(siz, 0.0f, siz);
                Vector3 pos = A[i, j].transform.position;
                float rot = 0.0f;
                bool can = true;
                float x = 0.0f, y = 0.0f;
                for(int l = 0; l < ec.Count; l++)
                {
                    int dist = (int)((X[l] - pos.x) * (X[l] - pos.x) + (Y[l] - pos.y) * (Y[l] - pos.y));
                    if (dist == 0) { can = false; break; }
                    float tmp = ck * Q[l] / dist;
                    x += Q[l] * (X[l] - pos.x) / (float)Math.Pow(dist, 1.5f);
                    y += Q[l] * (Y[l] - pos.y) / (float)Math.Pow(dist, 1.5f);
                    ans.y += tmp;
                }
                rot = (float)Math.Atan2(x, y);
                rot *= Mathf.Rad2Deg;
                ans.y = Math.Abs(ans.y);
                if (can && ans.y > 0)
                {
                    A[i, j].transform.GetChild(0).localScale = ans;
                    A[i, j].transform.GetChild(1).localScale = new Vector3(siz, siz, siz);
                    A[i, j].transform.GetChild(1).localPosition = new Vector3(0.0f, ans.y / 2, 0.0f);
                    A[i, j].transform.eulerAngles = new Vector3(0, 0, -rot+180.0f);
                }
                else
                {
                    A[i, j].transform.GetChild(0).localScale = new Vector3(0, 0, 0);
                    A[i, j].transform.GetChild(1).localScale = new Vector3(0, 0, 0);
                }
            }
        }
    }
    public void ud(int idx, int x, int y, int q)
    {
        print(idx + " " + x + " " + y + " " + q);
        X[idx] = x;
        Y[idx] = y;
        Q[idx] = q;
        B[idx].transform.position = new Vector3(x, y, 0);
        B[idx].transform.localScale = new Vector3((float)Math.Abs(q) / 10, (float)Math.Abs(q) / 10, (float)Math.Abs(q) / 10);
        if(q < 0) B[idx].GetComponent<MeshRenderer>().material = Bmat;
        else B[idx].GetComponent<MeshRenderer>().material = Rmat;
        cal();
    }
    public void add()
    {
        GameObject tmp = Instantiate(obj, scroll.transform);
        tmp.name = ec.Count.ToString();
        X.Add(0);
        Y.Add(0);
        Q.Add(0);
        ec.Add(tmp);
        tmp = Instantiate(ball);
        tmp.transform.position = new Vector3(0, 0, 0);
        B.Add(tmp);
        cal();
    }
    public void del(int i)
    {
        Destroy(B[i]);
        ec.RemoveAt(i);
        X.RemoveAt(i);
        Y.RemoveAt(i);
        Q.RemoveAt(i);
        B.RemoveAt(i);
        for(int idx = 0; idx < ec.Count; idx++)
            ec[idx].name = idx.ToString();
        cal();
    }
    public void cl()
    {
        foreach (GameObject go in B) Destroy(go);
        foreach (GameObject go in ec) Destroy(go);
        ec.Clear();
        X.Clear();
        Y.Clear();
        Q.Clear();
        B.Clear();
        cal();
    }
}
