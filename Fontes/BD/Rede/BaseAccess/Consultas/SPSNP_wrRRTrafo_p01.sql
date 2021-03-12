    INSERT INTO
      RRTrafo ( fltZ0R, fltZ0X, fltZ1R, fltZ1X, blbMatY,
                intPriBarraId, intPriTipoLigacaoId, intPriTipoFaseId, fltPriVnom, fltPriSnom, fltPriZterraR, fltPriZterraX,
                intSecBarraId, intSecTipoLigacaoId, intSecTipoFaseId, fltSecVnom, fltSecSnom, fltSecZterraR, fltSecZterraX,
                fltRotFase, intModeloEqv,
                intEstudoId, intMRedeId, intRRTrafoId, intCorteId
              )
    VALUES
     ( [p_Z0R], [p_Z0X], [p_Z1R], [p_Z1X], [p_MatY],
       [p_Barra1Id], [p_Enrol1_TipoLigacaoId], [p_Enrol1_TipoFaseId], [p_Enrol1_Vnom], [p_Enrol1_Snom], [p_Enrol1_ZterraR], [p_Enrol1_ZterraX],
       [p_Barra2Id], [p_Enrol2_TipoLigacaoId], [p_Enrol2_TipoFaseId], [p_Enrol2_Vnom], [p_Enrol2_Snom], [p_Enrol2_ZterraR], [p_Enrol2_ZterraX],
       [p_RotFase], [p_ModeloEqv],
       [p_EstudoId], [p_MRedeId], [p_RRTrafoId], [p_CorteId]
     );
