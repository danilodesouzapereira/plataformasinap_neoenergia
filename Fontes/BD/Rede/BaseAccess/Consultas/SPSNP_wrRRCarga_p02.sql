    UPDATE
      (SELECT * FROM RRCarga) sel
    SET
      sel.fltIcte_pu = [p_Icte_pu],
      sel.fltScte_pu = [p_Scte_pu],
      sel.fltZcte_pu = [p_Zcte_pu]
    WHERE
      sel.intEstudoId = [p_EstudoId] AND
      sel.intMRedeId = [p_MRedeId] AND
      sel.intRRCargaId = [p_RRCargaId] AND
      sel.intCorteId = [p_CorteId];
