using System.Collections;
using System.Collections.Generic;
using Pathfinding;
using UnityEngine;


public class EnemyTracking : MonoBehaviour
{

    public Transform target;
    public GameObject targetDataPort;
    
    public float speed = 200f;
    public float nextWaypointDistance = 3f;

    public float stopChasingIfDistance = 10f;

    public Transform EnemyGFX;

    public Transform[] moveSpots;
    private int nextSpot;

    public float startWaitTime;
    float waitTime;

    public float startAttackCooldown;
    float attackCooldown;

    Path path;
    int currentWaypoint = 0;
    bool reachedEndOfPath = false;

    Seeker seeker;
    Rigidbody2D thisEnemy;

    PlayerData playerStats;
    EnemyData enemyStats;

    void Start()
    {

        seeker = GetComponent<Seeker>();
        thisEnemy = GetComponent<Rigidbody2D>();
        nextSpot = Random.Range(0, moveSpots.Length);
        waitTime = startWaitTime;
        attackCooldown = 0;

        playerStats = targetDataPort.GetComponent<PlayerData>();
        enemyStats = this.GetComponent<EnemyData>();
        

        
        //if(distancebetweentarget < stopChasingIfDistance)
        //{
        InvokeRepeating("UpdatePath", 0f, .5f);
        //}

        //if(distancebetweentarget > stopChasingIfDistance)
        //{
        //    StartPatrol();
        //}
        
        
    
    }

    void UpdatePath()
    {

        float distancebetweentarget = Vector2.Distance(thisEnemy.position, target.position);


        if (distancebetweentarget < stopChasingIfDistance)
        {
            if(seeker.IsDone())
            seeker.StartPath(thisEnemy.position, target.position, OnPathComplete);

            if(Vector2.Distance(thisEnemy.position, target.position) < 0.92f)
            {

                //Initiate Attack
                if(attackCooldown <= 0)
                {

                    playerStats.currenthp -= enemyStats.damage;
                    attackCooldown = startAttackCooldown;

                }
                else{
                
                    attackCooldown -= 0.75f;

                }

            }
    
        }
        else if (distancebetweentarget > stopChasingIfDistance)
        {
            if(seeker.IsDone())
            seeker.StartPath(thisEnemy.position, moveSpots[nextSpot].position, OnPathComplete);

            if(waitTime <= 0)
            {
                nextSpot = Random.Range(0, moveSpots.Length);
                waitTime = startWaitTime;
            }
            else
            {
                waitTime -= 0.55f;
                
            }
        }
    }

    void OnPathComplete(Path p)
    {
        if (!p.error)
        {
            path = p;
            currentWaypoint = 0;
        }
    }

 

    void FixedUpdate()
    {

        

        if (path == null){
            return;
        }

        if (currentWaypoint >= path.vectorPath.Count)
        {
            reachedEndOfPath = true;
            return;
        } else
        {
            reachedEndOfPath = false;
        }

        Vector2 direction = ((Vector2)path.vectorPath[currentWaypoint] - thisEnemy.position).normalized;
        Vector2 force = direction * speed * Time.deltaTime;

        float distance = Vector2.Distance(thisEnemy.position, path.vectorPath[currentWaypoint]);

        

        thisEnemy.AddForce(force);


        

        if (distance < nextWaypointDistance)
        {
            currentWaypoint++;
        }

        if (thisEnemy.velocity.x >= 0.01f)
        {
            EnemyGFX.localScale = new Vector3(-1f, 1f, 1f);
        }
        else if (thisEnemy.velocity.x <= -0.01f)
        {
            EnemyGFX.localScale = new Vector3(1f, 1f, 1f);
        }

    }

}