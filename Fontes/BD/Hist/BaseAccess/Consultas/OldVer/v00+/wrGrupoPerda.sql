INSERT INTO
  GrupoPerda (intGrupoId, intGrupoPaiId, strDescricao, intTipoGrupoRedeId)
VALUES
  ([p_GrupoId], IIF([p_GrupoPaiId] = -1, NULL, p_GrupoPaiId), [p_Descricao], [p_TipoGrupoRedeId])
  