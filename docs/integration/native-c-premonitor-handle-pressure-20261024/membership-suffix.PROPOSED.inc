/* Exact membership suffix extracted from current ps_bd_census; proposal only. */
static DWORD proposed_member(const PS_BD_DESCRIPTOR *d,const PS_BD_INVENTORY *v,DWORD pid,DWORD parent,unsigned project)
{unsigned i;int child=0;
 for(i=0;i<PS_BD_ROLES;i++)if(v->present[i]){if(v->role[i].pid==pid){if(d->n0&&i==PS_BD_CONSOLE&&parent!=d->auxiliary.parent)return ERROR_INVALID_DATA;return 0;}if(v->role[i].pid==parent)child=1;}
 return child||project?ERROR_BUSY:0; /* unchanged host baseline, never a name exemption */
}
